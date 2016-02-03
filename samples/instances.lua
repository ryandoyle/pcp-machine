colours = metric('sample.colour')

-- Single instances can be accessed using the external instance name as a key
print(colours['red'])

-- Instances can also be iterated over
for name, value in pairs(colours) do
  print('The instance ' .. name .. ' has the value ' .. value)
end
