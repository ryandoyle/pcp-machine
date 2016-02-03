# pcp-machine
> Lua scripting engine for [Performance Co-Pilot]

`pcp-machine` is a programmable query tool for PCP (Performance Co-Pilot). It allows a user query for PCP metrics in Lua and then program based on these metrics. `pcp-machine` tries to adhere to the Unix philosophy of being small and modular and is intended to be used in a pipeline with other tools.

If used for monitoring, `pcp-machine` lives in a similar space as `pmie(1)` (Performance Metrics Inference Engine). `pmie` is able to apply rules to metrics in a more specific DSL rather than a general purpose language like Lua. `pmie` also has the ability to compare metrics from different time intervals by storing historical metrics in memory. `pmie` is intended to be run as a daemon and execute actions when predefined conditions are met.

## Basic usage
`pcp-machine` provides a simple Lua API. These scripts are then executed with `pcp-machine`.


```lua
-- metric() fetches the metric from the local pmcd (pcp client daemon)
nprocs = metric('kernel.all.nprocs')
if nprocs > 100 then
  print('Processes are too high! ' .. nprocs)
end
```
You then run the script with pcp-machine:
```sh
$ pcp-machine myscript.lua
Processes are too high! 845
```
This will run the query against `localhost`. To run the script against a different host, you can specify the `-H` option.
```sh
$ pcp-machine -H other-host.example.com myscript.lua
Processes are too high! 1328
```
The Lua built-in `return` function can also be used to set the exit code for `pcp-machine`.
```lua
nprocs = metric('kernel.all.nprocs')
if nprocs > 100 then
  print('Processes are too high!:' .. nprocs)
  return 2
else
  print('Everything is OK')
end
```
```sh
$ pcp-machine myscript.lua
Processes are too high!: 843
$ echo $?
2
```
If no `return` is specified, `pcp-machine` will exit with `0`. If there is an error runnning the script, `pcp-machine` will exit with `125`
```sh
$ pcp-machine myscript.lua
Everything is OK
$ echo $?
0
```
## Advanced Usage
### Types
The `metric()` function will automatically convert the type of the metric. `metric()` will return numbers, strings and tables (used for instances). It is up to the client querying the metric to know the type they will be using. Lua is also quite loosely typed and will automatically convert numbers to strings as needed. 
```lua
function type_of_metric(m)
  return type(metric(m))
end

print(type_of_metric('sample.load'))
print(type_of_metric('sample.string.hullo'))
print(type_of_metric('sample.colour'))
```
will yield:
```sh
$ pcp-machine types.lua
number
string
table
```
Traditionally Lua has not had an integer type. All numbers are represented as double precision floating point numbers. For more information, read http://www.lua.org/pil/2.3.html.

### Instances
For metrics that have multiple instances, the `metric()` function return will a map of instance names to values (known as a table in Lua).
```lua
colours = metric('sample.colour')

-- Single instances can be accessed using the external instance name as a key
print(colours['red'])

-- Instances can also be iterated over
for name, value in pairs(colours) do
  print('The instance ' .. name .. ' has the value ' .. value)
end
```
will yield:
```sh
$ pcp-machine instances.lua 
109
The instance green has the value 210
The instance blue has the value 311
The instance red has the value 109
```
### Multiple hosts
A host can also be passed into the `query()` function via the named parameter, `host`. This allows a user to query against different hosts and build scripts based on multiple metrics.
```lua
host1_load = metric('kernel.all.load', {host = 'host1.example.com'})['1 minute']
host2_load = metric('kernel.all.load', {host = 'host2.example.com'})['1 minute']

if host1_load + host2_load > 10 then
  print('load is too high over cluster')
  return 2
else
  print('load is okay over cluster')
  return 0
end
```
### Injecting data
Environment variables can be used when a Lua script requires more information than just the host to run the `metric()` query against.
```lua
domain = os.getenv("DOMAIN")
db_load = metric('kernel.all.load', {host = 'database.' .. domain})['1 minute']
print(db_load)
```
```sh
$ DOMAIN=example.com .pcp-machine environment_variables.lua
0.15
```
### Integrations
`pcp-machine` is intended to be used in concert with other tools. `pcp-machine` was originally written with the intention to be executed with [Nagios].

Example nagios command
```
define command {
  command_name  check_load
  command_line  /usr/bin/pcp-machine -H $HOSTADDRESS$ /usr/share/pcp-machine/samples/check_load.lua
}
```
## Building and installing
`pcp-machine` requires CMake 2.8+, `libpcp3-dev` and `liblua5.1.0-dev`
```sh
git clone https://github.com/ryandoyle/pcp-machine.git
cd pcp-machine
cmake .
make
```
If you want to install the `pcp-machine` binary, man page and samples to your machine, do:
```sh
sudo make install
```
If you'd rather not `make install`, `pcp-machine` is standalone and can be packaged/installed manually.

   [performance co-pilot]: <http://pcp.io/>
   [nagios]: <https://www.nagios.org/>
