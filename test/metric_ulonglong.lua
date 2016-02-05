if metric('sample.ulonglong.hundred') ~= 100 then
  print("metric does not match")
  return 2
end