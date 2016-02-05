if metric('sample.long.hundred', {host='localhost'}) ~= 100 then
  print("metric does not match")
  return 2
end