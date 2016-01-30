m = metric('disk.dev.read', {host='localhost'})


print(m['sda'])

for k, v in pairs(m) do
    print(k,v)
end

print(metric('disk.all.read'))

print(os.getenv("MYVAR"))

return 4
