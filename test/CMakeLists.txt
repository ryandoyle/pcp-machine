# Check returning different types from metric()
add_test(metric_long ${CMAKE_SOURCE_DIR}/pcp-machine ${CMAKE_CURRENT_SOURCE_DIR}/metric_long.lua)
add_test(metric_ulong ${CMAKE_SOURCE_DIR}/pcp-machine ${CMAKE_CURRENT_SOURCE_DIR}/metric_ulong.lua)
add_test(metric_longlong ${CMAKE_SOURCE_DIR}/pcp-machine ${CMAKE_CURRENT_SOURCE_DIR}/metric_longlong.lua)
add_test(metric_ulonglong ${CMAKE_SOURCE_DIR}/pcp-machine ${CMAKE_CURRENT_SOURCE_DIR}/metric_ulonglong.lua)
add_test(metric_float ${CMAKE_SOURCE_DIR}/pcp-machine ${CMAKE_CURRENT_SOURCE_DIR}/metric_float.lua)
add_test(metric_double ${CMAKE_SOURCE_DIR}/pcp-machine ${CMAKE_CURRENT_SOURCE_DIR}/metric_double.lua)
add_test(metric_string ${CMAKE_SOURCE_DIR}/pcp-machine ${CMAKE_CURRENT_SOURCE_DIR}/metric_string.lua)
add_test(metric_instances ${CMAKE_SOURCE_DIR}/pcp-machine ${CMAKE_CURRENT_SOURCE_DIR}/metric_instances.lua)

# Check passing in a host
add_test(metric_host ${CMAKE_SOURCE_DIR}/pcp-machine ${CMAKE_CURRENT_SOURCE_DIR}/metric_host.lua)
add_test(metric_host_option ${CMAKE_SOURCE_DIR}/pcp-machine -H localhost ${CMAKE_CURRENT_SOURCE_DIR}/metric_host_option.lua)

# Check return codes
add_test(return_default ${CMAKE_SOURCE_DIR}/test/check_return_code.sh ${CMAKE_CURRENT_SOURCE_DIR}/return_default.lua 0)
add_test(return_custom  ${CMAKE_SOURCE_DIR}/test/check_return_code.sh ${CMAKE_CURRENT_SOURCE_DIR}/return_custom.lua 25)
add_test(return_parse_fail ${CMAKE_SOURCE_DIR}/test/check_return_code.sh ${CMAKE_CURRENT_SOURCE_DIR}/return_parse_fail.lua 125)
add_test(metric_fail ${CMAKE_SOURCE_DIR}/test/check_return_code.sh ${CMAKE_CURRENT_SOURCE_DIR}/metric_fail.lua 125)

# Check output
add_test(printf ${CMAKE_SOURCE_DIR}/test/check_output.sh ${CMAKE_CURRENT_SOURCE_DIR}/printf.lua "s: str, f: 1.23")
