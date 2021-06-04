$=jQuery;

$(function() {
  updatePreviewContainer();
  
  $(".slider").change(function() {
    updatePreviewContainer();
  });
});

function updatePreviewContainer(){
    var redValue = $("#redSlider").val();
    var greenValue = $("#greenSlider").val();
    var blueValue = $("#blueSlider").val();
        
    var rgbValue = "rgb("+redValue+"," + greenValue + "," + blueValue + ")";
    $(".preview").css("background-color", rgbValue);
}

function sendValues(){
  var redValue = $("#redSlider").val();
  var greenValue = $("#greenSlider").val();
  var blueValue = $("#blueSlider").val();
  var brightnessValue = $("#brightnessSlider").val();
    
  
  var url = "http://192.168.1.81/rgb?r="+pad(redValue,3)+"&g="+pad(greenValue,3)+"&b="+pad(blueValue,3)+"&h="+pad(brightnessValue,3);
  $.ajax({
    url: url,
    context: document.body
  });
}

function pad(n, width, z) {
  z = z || '0';
  n = n + '';
  return n.length >= width ? n : new Array(width - n.length + 1).join(z) + n;
}