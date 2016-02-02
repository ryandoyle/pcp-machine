-- Will query for a metric against the host specified via -H option
nprocs = metric('kernel.all.nprocs')
if nprocs > 100 then
  print('Processes are too high!:' .. nprocs)
end

-- If no return is given, pcp-machine will exit with an exit status of 0
