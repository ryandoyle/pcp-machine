if metric('sample.string.hullo') ~= 'hullo world!' then
  print("metric does not match")
  return 2
end