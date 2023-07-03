from flask import Flask, render_template, request
from redis import Redis

app = Flask(__name__)

# define db connection
redis = Redis(host='redis', port=6379, decode_responses=True)

@app.route('/add/game', methods = ['POST'])
def add_score():
    # # store data from the post request in variables
    # print(request)
    gameId = request.form.get('gameId')
    duration = request.form.get('duration')
    score = request.form['score']
    name = request.form['name']
    # # key is game_id
    # # duration, score, and name stored in game_id key
    # # can be read with redis.hget(key, "duration") etc.
    redis.hset(gameId, key="duration", value=duration)
    redis.hset(gameId, key="score", value=score)
    redis.hset(gameId, key="name", value=name)
   
   # return success or user
    return f"gameId = {gameId}, duration = {duration}, score = {score}, name = {name}"

# create route for getting a specific game score
# get gameId from request args request.args
# use gameId to pull data from redis
# return user data

@app.route('/get/game', methods =['GET'])
def get_score():
    gameId = request.args['gameId']
    duration = redis.hget(gameId, "duration")
    score = redis.hget(gameId, "score")
    name = redis.hget(gameId, "name")
    return f"{duration}, {score}, {name}"

@app.route('/')
def loadHome():
    return render_template('home.html')

@app.route('/about/')
def loadAbout():
    return render_template('about.html')

if __name__ == '__main__':
   app.run(host='0.0.0.0', debug=True)