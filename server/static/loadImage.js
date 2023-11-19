const LOAD_DIRECT = true;

function loadPolarImage() {
  $.ajax({
    url: "/image/polar",
    type: "GET",
    success: function (data) {
      if (typeof data.image === "string") {
        if (data.image.length === 0) {
          return;
        }

        $("#polar img").attr("src", `data:image/png;base64,${data.image}`);
      }
    },
  });
}

function loadLatestPolarImage() {
  $.ajax({
    url: "/image/polar/latest",
    type: "GET",
    success: function (data) {
      if (typeof data.image === "string") {
        if (data.image.length === 0) {
          return;
        }

        $("#polar_latest img").attr(
          "src",
          `data:image/png;base64,${data.image}`
        );
      }
    },
  });
}

function loadCartesianImage() {
  $.ajax({
    url: "/image/cartesian",
    type: "GET",
    success: function (data) {
      if (typeof data.image === "string") {
        if (data.image.length === 0) {
          return;
        }

        $("#cartesian img").attr("src", `data:image/png;base64,${data.image}`);
      }
    },
  });
}

function loadCalibrationImage() {
  $.ajax({
    url: "/image/calibration",
    type: "GET",
    success: function (data) {
      if (typeof data.image === "string") {
        if (data.image.length === 0) {
          return;
        }

        $("#calibration img").attr(
          "src",
          `data:image/png;base64,${data.image}`
        );
      }
    },
  });
}

$(document).ready(function () {
  $("#polar").html('<img src="/image/polar/direct" />');
  $("#polar_latest").html('<img src="/image/polar/latest/direct" />');
  $("#cartesian").html('<img src="/image/cartesian/direct" />');
  $("#calibration").html('<img src="/image/calibration/direct" />');
});

let isVisible = true;

document.addEventListener("visibilitychange", () => {
  if (document.visibilityState === "visible") {
    isVisible = true;
  } else {
    isVisible = false;
  }
});

window.setInterval(function () {
  if (!isVisible) {
    return;
  }

  if (LOAD_DIRECT) {
    loadPolarImage();
    loadLatestPolarImage();
    loadCartesianImage();
    loadCalibrationImage();
  } else {
    $("#polar").html('<img src="/image/polar/direct" />');
    $("#polar_latest").html('<img src="/image/polar/latest/direct" />');
    $("#cartesian").html('<img src="/image/cartesian/direct" />');
    $("#calibration").html('<img src="/image/calibration/direct" />');
  }
}, 250);
