-- Query against a different host instead of the one specified via -H 
-- command line option
free_space = metric('filesys.free', {host='myhost.example.com'})

-- For metrics that have instances, the returned value is a table 
-- (basically a dictionary/map). Values are keyed off their external
-- instance identifier. EG: "/dev/hda1". If we wanted to access a
-- an instance of a metric directly we could use free_space['/dev/hda1']
-- instead of iterating over all the instances
for disk, bytes_free in pairs(free_space) do
  if bytes_free < 10000000 then
    print('Space on disk' .. instance .. 'is too low!')
    -- Returning early here will stop execution in pcp-machine and
    -- return this exit code. This can be useful when pcp-machine is
    -- used in alerting tools like Nagios
    return 2
  end
end

print "Disks are all okay!"
return 0
