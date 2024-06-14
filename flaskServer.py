from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route("/sensor/dht", methods=["POST"])
def receive_data():
  if request.method == "POST":
    temperature = request.form["temperature"]
    humidity = request.form["humidity"]

    response = {
      "message": "Data received successfully",
      "temperature": temperature,
      "humidity": humidity
    }

    return jsonify(response), 200

if __name__ == "__main__":
  app.run(debug=True, host="0.0.0.0", port=5050)
