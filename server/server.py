import matplotlib
import matplotlib.pyplot as plt
import math
from flask import Flask, request, render_template, send_file
import json
import os

matplotlib.use('agg')

app = Flask(__name__)

@app.route("/upload", methods=['POST'])
def addPlot():
    data = json.loads(request.data)
    angles = data['angle']
    distances = data['distance']

    #print(angles)
    print(distances[0])

    # Convert degrees to radians
    for i in range(len(angles)):
        angles[i] = math.radians(angles[i])

    plt.clf()
    plt.polar(angles, distances, 'ro')
    plt.savefig('./static/map.png')
    
    return json.dumps({'status': 1})

@app.route("/", methods=['GET'])
def showPlot():
    return render_template("index.html")

@app.route("/image", methods=['GET'])
def get_image():
    return send_file("./static/map.png", mimetype='image/gif')

@app.after_request
def add_header(response):
    # response.cache_control.no_store = True
    response.headers['Cache-Control'] = 'no-store, no-cache, must-revalidate, post-check=0, pre-check=0, max-age=0'
    response.headers['Pragma'] = 'no-cache'
    response.headers['Expires'] = '-1'
    return response

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8080, debug=True)
    app.config['SEND_FILE_MAX_AGE_DEFAULT'] = 0
