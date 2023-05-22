from flask import Flask, request
from redis import Redis

app = Flask(__name__)

# define db connection
redis = Redis(host='redis', port=6379, decode_responses=True)

@app.route('/add/user', methods = ['POST'])
def add_user():
    # store data from the post request in variables
    user_id = request.form['user_id']
    name = request.form['name']
    age = request.form.get('age', None)



    # key is user_id
    # name and age stored in user_id key
    # can be read with redis.hget(key, "name") etc.
    redis.hset(user_id, key="name", value=name)
    redis.hset(user_id, key="age", value=age)
   
   # return success or user
    return user_id;
# create route for getting user
# get user from request args request.args
# use user to pull data from redis
# return user data

@app.route('/get/user', methods =['GET'])
def get_user():
    user_id = request.args['user_id']
    name = redis.hget(user_id, "name")
    age = redis.hget(user_id, "age")



    return f"{user_id}, {name}, {age}"

if __name__ == '__main__':
   app.run(host='0.0.0.0', debug=True)