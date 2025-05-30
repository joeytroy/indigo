// Copyright (c) 2024 Moravian Instruments
// All rights reserved.
//
// You can use this software under the terms of 'INDIGO Astronomy
// open-source license' (see LICENSE.md).
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHORS 'AS IS' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// version history
// 2.0 by Jakub Smutny <info@gxccd.com>

/** INDIGO Moravian Instruments SFW driver main
 \file indigo_wheel_mi_main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <indigo/indigo_driver_xml.h>
#include "indigo_wheel_mi.h"

int main(int argc, const char * argv[]) {
	indigo_main_argc = argc;
	indigo_main_argv = argv;
	/* Executable drivers use pipes - no HTTP */
	indigo_use_blob_urls = false;
	indigo_client *protocol_adapter = indigo_xml_device_adapter(0, 1);
	indigo_enable_blob_mode_record *record = (indigo_enable_blob_mode_record *)malloc(sizeof(indigo_enable_blob_mode_record));
	memset(record, 0, sizeof(indigo_enable_blob_mode_record));
	record->mode = INDIGO_ENABLE_BLOB_ALSO;
	protocol_adapter->enable_blob_mode_records = record;
	indigo_start();
	indigo_wheel_mi(INDIGO_DRIVER_INIT, NULL);
	indigo_attach_client(protocol_adapter);
	indigo_xml_parse(NULL, protocol_adapter);
	indigo_wheel_mi(INDIGO_DRIVER_SHUTDOWN, NULL);
	indigo_stop();
	return 0;
}
