nprocs = metric('kernel.all.nprocs')
if nprocs > 100 then
  print('Processes are too high!:' .. nprocs)
end
