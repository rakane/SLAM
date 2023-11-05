$(document).ready(function () {
  $("#polar").html('<img src="/image/polar"/>');
  $("#cartesian").html('<img src="/image/cartesian"/>');
});

let isVisible = true;

document.addEventListener("visibilitychange", () => {
  if (document.visibilityState === "visible") {
    isVisible = true;
    $("#polar").html('<img src="/image/polar"/>');
    $("#cartesian").html('<img src="/image/cartesian"/>');
  } else {
    isVisible = false;
  }
});

window.setInterval(function () {
  if (!isVisible) {
    return;
  }

  $.ajax({
    url: "/image/polar",
    type: "GET",
    success: function (data) {
      $("#polar img").attr("src", `data:image/png;base64,${data.image}`);
    },
  });

  $.ajax({
    url: "/image/cartesian",
    type: "GET",
    success: function (data) {
      $("#cartesian img").attr("src", `data:image/png;base64,${data.image}`);
    },
  });
}, 1000);
