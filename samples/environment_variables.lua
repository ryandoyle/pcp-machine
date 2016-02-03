domain = os.getenv("DOMAIN")
db_load = metric('kernel.all.load', {host = 'database.' .. domain})['1 minute']
print(db_load)
