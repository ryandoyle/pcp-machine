m = metric('disk.dev.read', 'localhost')

print(m['sda'])

for k, v in pairs(m) do
    print(k,v)
end

print(metric('disk.all.read'))

return 123
