function sendCommand(command) {
  $.ajax({
    type: "POST",
    url: "/command",
    data: JSON.stringify({ command: command }),
    contentType: "application/json",
    dataType: "json",
    success: function (data) {
      if (data.status === 0) {
        alert("Failed to send command");
      }
    },
    error: function (error) {
      console.log(error);
    },
  });
}

$(document).ready(function () {
  body = document.getElementsByTagName("body")[0];
  body.addEventListener("keydown", function (e) {
    // WASD for movement
    // Space for stop
    // E for exit

    if (e.keyCode == 87) {
      sendCommand("F");
    } else if (e.keyCode == 83) {
      sendCommand("B");
    } else if (e.keyCode == 65) {
      sendCommand("L");
    } else if (e.keyCode == 68) {
      sendCommand("R");
    } else if (e.keyCode == 32) {
      sendCommand("S");
    } else if (e.keyCode == 69) {
      sendCommand("E");
    }
  });

  $("#forward").click(function () {
    sendCommand("F");
  });

  $("#backward").click(function () {
    sendCommand("B");
  });

  $("#left").click(function () {
    sendCommand("L");
  });

  $("#right").click(function () {
    sendCommand("R");
  });

  $("#stop").click(function () {
    sendCommand("S");
  });

  $("#exit").click(function () {
    sendCommand("E");
  });
});
