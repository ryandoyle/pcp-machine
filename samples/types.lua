function type_of_metric(m)
  return type(metric(m))
end

print(type_of_metric('sample.load'))
print(type_of_metric('sample.string.hullo'))
print(type_of_metric('sample.colour'))
