function sendCommand(command) {
  $.ajax({
    type: "POST",
    url: "/command",
    data: JSON.stringify({ command: command }),
    contentType: "application/json",
    dataType: "json",
    success: function (data) {
      // console.log(data);
    },
    error: function (error) {
      console.log(error);
    },
  });
}

$(document).ready(function () {
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
