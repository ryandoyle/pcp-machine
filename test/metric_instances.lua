ints = metric('sample.many.int')

if ints['i-0'] ~= 0 then
  print("metric does not match")
  return 2
end

if ints['i-1'] ~= 1 then
  print("metric does not match")
  return 2
end

if ints['i-2'] ~= 2 then
  print("metric does not match")
  return 2
end

if ints['i-3'] ~= 3 then
  print("metric does not match")
  return 2
end

if ints['i-4'] ~= 4 then
  print("metric does not match")
  return 2
end