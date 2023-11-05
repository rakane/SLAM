import matplotlib
import matplotlib.pyplot as plt
import math
from flask import Flask, request, render_template, send_file, jsonify
import json
import os
import base64
import time

matplotlib.use('agg')

app = Flask(__name__)

@app.route("/upload", methods=['POST'])
def upload_map():
    # Get performance start time
    start = time.time()

    data = json.loads(request.data)
    angles = data['angle']
    distances = data['distance']
    x = data['x']
    y = data['y']
    resolutionMillis = data['resolution']

    ########################################
    # Polar plot
    ########################################
    # Convert degrees to radians
    for i in range(len(angles)):
        angles[i] = math.radians(angles[i])

    # Convert mm to m
    for i in range(len(distances)):
        distances[i] = distances[i] / 1000

    # Remove last element from both arrays
    angles.pop()
    distances.pop()

    if(len(angles) != len(distances)):
        return json.dumps({'status': 0})
    
    if(len(angles) == 0):
        return json.dumps({'status': 0})

    plt.clf()
    plt.polar(angles, distances, 'ro', markersize=2)
    plt.title('Lidar Scan in meters')
    plt.savefig('./static/polar_map.png')

    ########################################
    # Cartesian plot
    ########################################
     # Remove last element from both arrays
    x.pop()
    y.pop()

    if(len(x) != len(y)):
        return json.dumps({'status': 0})
    
    if(len(x) == 0):
        return json.dumps({'status': 0})

    # Convert mm to m
    for i in range(len(x)):
        x[i] = x[i] / 1000
        y[i] = y[i] / 1000

    plt.clf()
    plt.plot(x, y, 'sb', markersize=2)
    plt.axis('equal')
    plt.xlabel('X (m)')
    plt.ylabel('Y (m)')

    # Set grid to show every 100mm
    plt.grid(which='major', color='#666666', linestyle='-')
    plt.title('Mapping with resolution of ' + str(resolutionMillis) + 'mm')
    plt.savefig('./static/cartesian_map.png')
    
    # Get performance end time
    end = time.time()

    print("Time taken: " + str(end - start) + " seconds")

    return json.dumps({'status': 1})

@app.route("/", methods=['GET'])
def show_plot():
    return render_template("index.html")

@app.route("/image/polar", methods=['GET'])
def get_polar_image():
    # Open image
    file = open("./static/polar_map.png", "rb")

    if not file:
        return jsonify({'image': ''})

    # Get file update time
    file_update_time = os.path.getmtime("./static/polar_map.png")

    # Send as base64 encoded image
    data = file.read()
    data = base64.b64encode(data).decode()

    return jsonify({'image': data, 'last_updated': file_update_time})

@app.route("/image/cartesian", methods=['GET'])
def get_cartesian_image():
    # Open image
    file = open("./static/cartesian_map.png", "rb")

    if not file:
        return jsonify({'image': ''})
    
    # Get file update time
    file_update_time = os.path.getmtime("./static/cartesian_map.png")

    # Send as base64 encoded image
    data = file.read()
    data = base64.b64encode(data).decode()

    return jsonify({'image': data, 'last_updated': file_update_time})

@app.route("/image/polar/direct", methods=['GET'])
def get_polar_image_direct():
    return send_file("./static/polar_map.png", mimetype='image/png')

@app.route("/image/cartesian/direct", methods=['GET'])
def get_cartesian_image_direct():
    return send_file("./static/cartesian_map.png", mimetype='image/png')

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
