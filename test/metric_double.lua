if metric('sample.double.hundred') ~= 100 then
  print("metric does not match")
  return 2
end