file(DOWNLOAD ${URL} ${OUTPUT} STATUS status)
list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
    message(WARNING "Failed to download file: ${status_string}")
endif()