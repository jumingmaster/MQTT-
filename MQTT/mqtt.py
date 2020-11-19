#/usr/bin/python3
from influxdb_client import InfluxDBClient, Point


# 创建一个bucket
def create_bucket(db:str, policy:str):
    username = ' ' # 无
    password = ' ' # 无
    database = db # 数据库
    retention_policy = policy # policy
    bucket = f'{database}/{retention_policy}' # 2.0版本的概念 
    return bucket


# 创建一个client实例
def influxDB_new(bucket, Url = 'http://localhost:8086', username = ' ', password = ' ', org = '-'):
    client = InfluxDBClient(Url, token=f'{username}:{password}', org='-')
    return client


# 创建一个写入实例
def create_writeOBJ(client):
    # 创建写入实例
    write_api = client.write_api()
    return write_api



# 获得一个point
def get_point(measurement, tags:str, tags_key:str, fields:str, value):
    # 获得一个point
    point = Point(measurement).tag(tags, tags_key).field(fields, value)
    return point


# 写入数据库
def write_data_to_DB(point, write_api, buckets):
    print('***Write Points***')
    print(point.to_line_protocol())
    # 写入
    write_api.write(bucket=buckets, record=point)
    print('*******************')
        

# 销毁write_api对象
def destory_write_api(write_api):
    write_api.__del__()


# 销毁client对象
def destory_client(client):
    client.close()


