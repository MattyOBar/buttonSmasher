
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

# create route for API
# get number of keys in redis database
# use number of keys to know how many keys to iterate through keys with a duration of 15
# iterate through the ids and pull the score/name out into a nested list
# sort the list of score/names in descending order by score.
# return the sorted list

@app.route('/get/15', methods=['GET'])
def get_ids_duration_15():
    duration_15_ids = []
    scores_and_names_list = []
    
    num_of_keys = str(redis.dbsize())
    
    for i in range(int(num_of_keys)):
        duration = redis.hget(i, "duration")
        if duration == str(15): 
            duration_15_ids.append(i)
            
    for k in range(len(duration_15_ids)):
        score = redis.hget(duration_15_ids[k], "score")
        name = redis.hget(duration_15_ids[k], "name")
        newList = []
        newList = [score, name]
        scores_and_names_list.append(newList)
    
    if (len(scores_and_names_list) == 0):
        return "Scores are empty.  Please play a game."
    
    sorted_scores_and_names_list_15 = sort_list(scores_and_names_list)
    
    return sorted_scores_and_names_list_15

@app.route('/get/30', methods=['GET'])
def get_ids_duration_30():
    duration_30_ids = []
    scores_and_names_list = []
    
    num_of_keys = str(redis.dbsize())
    
    for i in range(int(num_of_keys)):
        duration = redis.hget(i, "duration")
        if duration == str(30): 
            duration_30_ids.append(i)
            
    for k in range(len(duration_30_ids)):
        score = redis.hget(duration_30_ids[k], "score")
        name = redis.hget(duration_30_ids[k], "name")
        newList = []
        newList = [score, name]
        scores_and_names_list.append(newList)
    
    if (len(scores_and_names_list) == 0):
        return str("Scores are empty.  Please play a game.")
    
    sorted_scores_and_names_list_30 = sort_list(scores_and_names_list)
    
    return sorted_scores_and_names_list_30
    
@app.route('/')
def load_home(list=None):
    return render_template('home.html', list=get_ids_duration_15())

@app.route('/about/')
def load_about():
    return render_template('about.html')

# this is a helper function, used to sort the score/name key/values in descending order
def sort_list(list):
    sorted_list = sorted(list, key=lambda x: (int(x[0]), x[1]), reverse=True)
    return sorted_list

if __name__ == '__main__':
   app.run(host='0.0.0.0', debug=True)