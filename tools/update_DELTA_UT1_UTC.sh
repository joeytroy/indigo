#/bin/sh

DATA_SOURCE="https://datacenter.iers.org/products/eop/rapid/daily/csv/finals2000A.daily.csv"


LATEST_MEASURED_LINE="$(curl -s "$DATA_SOURCE" | awk -F ';' '$14 == "final" {last = $0} END {print last}')"

# echo "Latest: $LATEST_MEASURED_LINE"

YEAR="$(echo "$LATEST_MEASURED_LINE" | awk -F ';' '{print $2}')"
MONTH="$(echo "$LATEST_MEASURED_LINE" | awk -F ';' '{print $3}')"
DAY="$(echo "$LATEST_MEASURED_LINE" | awk -F ';' '{print $4}')"
DELTA_UT1_UTC="$(echo "$LATEST_MEASURED_LINE" | awk -F ';' '{print $15}')"

echo "DELTA_UT1_UTC = $DELTA_UT1_UTC for date $YEAR-$MONTH-$DAY"

if ! echo "$DELTA_UT1_UTC" | grep -Eq '^-?[0-9]+(\.[0-9]+)?$'; then
	echo "Error: DELTA_UT1_UTC is not a valid number: $DELTA_UT1_UTC"
	exit 1
fi

if (( $(echo "$DELTA_UT1_UTC < -1 || $DELTA_UT1_UTC > 1" | bc -l) )); then
	echo "Error: DELTA_UT1_UTC is out of the expected range (-1 to 1): $DELTA_UT1_UTC"
	exit 1
fi

for file in \
		indigo_libs/indigo/indigo_align.h \
		indigo_libs/indigo/indigo_driver.h \
		indigo_libs/indigocat/indigocat_ss.c \
		indigo_libs/indigocat/example/planets_test.c; do
	if ! sed -i "s%#define DELTA_UT1_UTC.*%#define DELTA_UT1_UTC ($DELTA_UT1_UTC / 86400.0) /* For $YEAR-$MONTH-$DAY */%g" "$file"; then
		echo "Error: Failed to update $file"
		exit 1
	fi
done

echo "DELTA_UT1_UTC successfully updated"
