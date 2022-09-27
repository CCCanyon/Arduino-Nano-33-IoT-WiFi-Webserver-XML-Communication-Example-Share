const char HTML_OK[] PROGMEM = R"=====(
HTTP/1.1 200 OK
Content-type:text/html
)=====";

const char XMLparam0[] PROGMEM = R"=====(
<?xml version="1.0"?>
)=====";
const char XMLparam1[] PROGMEM = R"=====(
)=====";

const char PAGE_MAIN[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en" class="js-focus-visible">
<title>Arduino Nano 33 IoT Page</title>
<head>
<style>
h1{text-align: center; font-family: "Lucida Console";}
.button
{
border: none; color: white; padding: 1vw 1vw; text-align: center; width: 100vw; 
text-decoration: none; display: inline-block; font-size: 5vw; margin: 1vw 1vw;
cursor: pointer; font-family: "Lucida Console"
}
.orange{background-color: #FF9000;}
.turq{background-color: #21BCA5;}
.blue{background-color: #495CFF;}
</style>
</head>
<body>
<h1>LED pin 13</h1>
<button type="button" class="button orange" id="ledbtn" onclick="commandXML('led', this.id)">BLINK</button>
<button type="button" class="button turq" id="test" onclick="requestXMLtoID('test', 'status')">TEST</button>
<p id="status"></p>
<p id="time"></p>
<br><br>
<form action="cal"><input class="button blue" type="submit" value="Calibration" /></form>
</body>
<script>
function commandXML(value, id)
{
  var xhttp = new XMLHttpRequest();
  xhttp.open("PUT", value, true);
  xhttp.send();  
}
function requestXMLtoID(value, id)
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById(id).innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", value, true);
  xhttp.send();  
}
function requestXML(value, fn)
{  
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      return fn(this);
    }
  };
  xhttp.open("GET", value, true);
  xhttp.overrideMimeType('application/xml');
  xhttp.send();  
}
function routine(xml)
{
  var x, t, xmlDoc;
  xmlDoc = xml.responseXML;
  x = xmlDoc.getElementsByTagName("time");
  t = x[0].childNodes[0].nodeValue;
  document.getElementById("time").innerHTML = t;
}
var run = setInterval("requestXML('update', routine)", 333);
</script>
</html>
)=====";







const char PAGE_CAL[] PROGMEM = R"=====(
<!DOCTYPE html>
<meta charset="UTF-8">
<html>
<head>
<style>
</style>

</head>
<body>
<form action="/action_page.php">
  <div>
      <caltext>PLAY</caltext>
      <input type="number" id="play_p_n" name="play_p" class="slider" value="0" min="0" max="1000" oninput="UpdateSlider(this.value)">
      <input type="range" id="play_p_s" class="slider" value="0" min="0" max="1000"><br><br>
  </div>
  <div>
      <caltext>PAUSE</caltext>
      <input type="number" id="pause_p_n" name="pause_p" class="slider" value="0" min="0" max="1000">
      <input type="range" id="pause_p_s" class="slider" value="0" min="0" max="1000"><br><br>
  </div>
  <div>
      <caltext>STOP</caltext>
      <input type="number" id="stop_p_n" name="stop_p" class="slider" value="0" min="0" max="1000">
      <input type="range" id="stop_p_s" class="slider" value="0" min="0" max="1000"><br><br>
  </div>
  <div>
      <caltext>RETRACT</caltext>
      <input type="number" id="retract_p_n" name="retract_p" class="slider" value="0" min="0" max="1000">
      <input type="range" id="retract_p_s" class="slider" value="0" min="0" max="1000">
  </div>
  <input class="button button3" type="submit" value="UPDATE">
</form>
<button type="button" class="button button3" id="btn0" onclick="ButtonPress0()">ButtonPress0</button>
<form action="home"><input class="button button3" type="submit" value="HOME" /></form>
</body>

<script type = "text/javascript">
var range1 = document.querySelector('#play_p_s');
var field1 = document.querySelector('#play_p_n');

var range2 = document.querySelector('#pause_p_s');
var field2 = document.querySelector('#pause_p_n');

var range3 = document.querySelector('#stop_p_s');
var field3 = document.querySelector('#stop_p_n');

var range4 = document.querySelector('#retract_p_s');
var field4 = document.querySelector('#retract_p_n');


range1.addEventListener('input', function (e) { field1.value = e.target.value; });
field1.addEventListener('input', function (e) { range1.value = e.target.value; });

range2.addEventListener('input', function (e) { field2.value = e.target.value; });
field2.addEventListener('input', function (e) { range2.value = e.target.value; });

range3.addEventListener('input', function (e) { field3.value = e.target.value; });
field3.addEventListener('input', function (e) { range3.value = e.target.value; });

range4.addEventListener('input', function (e) { field4.value = e.target.value; });
field4.addEventListener('input', function (e) { range4.value = e.target.value; });

var n = scaleValue(0, [-360,+360], [-1,1]);

function scaleValue(value, from, to)
{
  var scale = (to[1] - to[-1]) / (from[1] - from[-1]);
  var capped = Math.min(from[1], Math.max(from[1], value)) - from[0];
  console.log(capped);

}
</script>
</html>
)=====";

/*

h1
{
  text-align: center; font-family: "Lucida Console";
}
.button
{
  border: none;
  width: 210px;
  color: white;
  padding: 10px 10px;
  margin: 5px 5px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 50px;
  cursor: pointer;
  font-family: "Lucida Console"
}
.button3
{
  background-color: #495CFF;
}
caltext
{
  padding: 5px 5px;
  width: 210px;
  margin: 5px 5px;
  display: inline-block;
  font-size: 50px;
  text-align: center;
  font-family: "Lucida Console"
}
input[type="number"]
{
  width: 150px;
  height: 60px;
  padding: 5px 5px;
  margin: 5px 5px;
  display: inline-block;
  font-size: 50px;
  text-align: right;
  border: 2px solid #ccc;
  border-radius: 5px;
  box-sizing: border-box;
  -moz-appearance: textfield;
}
input[type="range"]
{
  -webkit-appearance: none;
  position: relative;
  top: 10px;
  padding: 10px 10px;
  margin: 5px 5px;
  width: 300px;
  background: rgba(255, 255, 255, 0.6);
  border-radius: 10px;
  background: #495CFF;
}
input[type="range"]::-webkit-slider-thumb
{
  -webkit-appearance: none;
  height: 40px;
  width: 40px;
  border-radius: 5px;
  background: #ffffff;
  cursor: pointer;
}
*/