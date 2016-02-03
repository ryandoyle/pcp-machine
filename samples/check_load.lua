load_avg = metric('kernel.all.load')['5 minute'] 

if load_avg > 10 then
  print('Load ' .. load_avg .. ' is too high')
  return 2
else
  print('Load ' .. load_avg .. ' is fine')
  return 0
end
