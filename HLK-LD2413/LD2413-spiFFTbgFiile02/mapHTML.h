void mapHTML(){
  HTMLstring=F("<html>\n<head>\n\
<meta name='viewport' content='width=device-width, initial-scale=0.5'>\n\
<style>\n\
body,table,input{font-size:30px; text-align:center}\n\
input[type=text]{font-size:30px; font-weight:bold}\n\
input[type=button]{font-size:30px; font-weight:bold}\n\
input[type=checkbox]{width:30px; height:30px;}\n\
.menu {position:absolute;left:100px; top:100px; visibility:hidden;\n\ 
  border-style:solid; border-color:#00ff00; width:500px; height:340px;\n\ 
  background-color:#ffffffff}\n\
</style>\n\
<script>\n\
function reloadPage() {window.location.reload();}\n\
screen.orientation.addEventListener('change',reloadPage);\n\
Run=true;\n\
//ws\n\
sampSize=1024; hSampSize=sampSize/2; dataSize=sampSize+hSampSize;\n\
distSize=hSampSize;\n\
dataFFT=[];\n\
Map=[];//Image\n\
Distance=12345.67;\n\ 
strData='';\n\
Rem='';\n\
configStr='';\n\
Start=true; DistOk=false; FFTdataOk=false;\n\
//-----------------Color-----------------------\n\
cMax=11;\n\
ColorMap=[[0, 0, 0, 0, 0, 1, 2, 6, 8,11,11,11],\n\
          [0, 0, 0, 2, 4, 5, 6, 5, 3, 0, 5,11],\n\
          [0, 1, 4, 5, 3, 0, 0, 0, 0, 0, 0,11]];\n\
function getColorT(a){\n\
  let ii=(a-Amin)/(Amax-Amin)*cMax;\n\
  let i=Math.floor(ii); if (i>=cMax) i=cMax-1;\n\
  let di=ii-i; let idi=1-di;\n\
  let R=ColorMap[0][i]*idi+ColorMap[0][i+1]*di;\n\ 
  let G=ColorMap[1][i]*idi+ColorMap[1][i+1]*di;\n\ 
  let B=ColorMap[2][i]*idi+ColorMap[2][i+1]*di;\n\ 
  R=R*255/cMax; G=G*255/cMax; B=B*255/cMax;\n\
  gr.fillStyle='rgb('+R+','+G+','+B+')';\n\
}//getColor\n\
function getColorA(a){\n\
  let c=(a-Amin)/(Amax-Amin)*255;\n\
  gr.fillStyle='rgb('+c+','+c+','+c+')';\n\
}//getColorA\n\
//----------------WS--------------------------\n\
ws=new WebSocket('ws://192.168.4.1:81/');\n\
ws.onmessage=function(msg){\n\
  ws.binaryType='arraybuffer';\n\
  if (msg.data instanceof ArrayBuffer){\n\
    if (msg.data.byteLength==dataSize) {\n\
      dataBuf=new Uint8Array(msg.data,0,dataSize);\n\
      dataFFT=new Int8Array(dataSize);\n\  
      for (i=0; i<distSize; i++) dataFFT[i]=dataBuf[sampSize+i];\n\ 
      if (Run) Draw();\n\
    }//if buffSize\n\ 
    if (msg.data.byteLength==4) {\n\
      buff=new Uint8Array(msg.data).buffer;\n\
      view=new DataView(buff);\n\
      Distance=view.getFloat32(0,true);\n\
      if (Run) dist.value=Distance.toFixed(2);\n\
    }//if bufSize==4\n\ 
  }//if msg.data\n\
  else {//string\n\ 
    configStr=msg.data;\n\
  }//else\n\ 
}//onmessage \n\
function fillBg(){ gr.fillStyle='#808080'; gr.fillRect(0,0,picW,picH);\n\
};\n\
function DrawL(){ gr.save();\n\
  if (_Grey.checked) getColor=getColorA; else getColor=getColorT;\n\
  Map=gr.getImageData(0,0,picW,picH); gr.putImageData(Map,0,-dTick); //move up\n\
  for (d=Dmin; d<Dmax; d++) { getColor(dataFFT[d]);\n\
    x=remap(d,Dmin,Dmax,Xmin,Xmax); dX=remap(d+1,Dmin,Dmax,Xmin,Xmax)-x;\n\
    gr.fillRect(x,picH,dX,-dTick);\n\
  };\n\
  gr.restore();\n\
}//DrawL\n\
function DrawP(){ gr.save();\n\
  if (_Grey.checked) getColor=getColorA; else getColor=getColorT;\n\
  Map=gr.getImageData(0,0,picW,picH); gr.putImageData(Map,dTick,0); //move right\n\
  for (d=Dmin; d<Dmax; d++) { getColor(dataFFT[d]);\n\
    y=remap(d,Dmin,Dmax,Ymin,Ymax); dY=remap(d+1,Dmin,Dmax,Ymin,Ymax)-y;\n\
    gr.fillRect(0,y,dTick,dY);\n\
  };\n\
  gr.restore();\n\
}//DrawP\n\
function remap(x,xMin,xMax,yMin,yMax){\n\
  return(yMin+(yMax-yMin)*(x-xMin)/(xMax-xMin));\n\
}//remap\n\
function showSet(){\n\
  _Dmin.value=Dmin; _Dmax.value=Dmax;\n\
  _Amin.value=Amin; _Amax.value=Amax;\n\
  _Rem.value=Rem;\n\
  _dTick.value=dTick;\n\ 
  set.style.visibility='visible';\n\
};\n\
function restoreSet(){\n\
   Din=0; Dmax=hSampSize; Amin=0; Amax=120; dTick=2;\n\
   Rem=''; Grey=false;\n\
  _Dmin.value=Dmin; _Dmax.value=Dmax;\n\
  _Amin.value=Amin; _Amax.value=Amax;\n\
  _dTick.value=dTick; _Grey.checked=Grey;\n\ 
  _Rem.value=Rem;\n\
  set.style.visibility='hidden';\n\
};\n\
function hideSet(){\n\
  Dmin=parseInt(_Dmin.value); Dmax=parseInt(_Dmax.value);\n\
  Amin=parseInt(_Amin.value); Amax=parseInt(_Amax.value);\n\
  dTick=parseInt(_dTick.value);\n\
  Grey=_Grey.checked;\n\
  if(Dmin<0) {Dmin=0; _Dmin.value=Dmin;}\n\ 
  if(Dmax>hSampSize) {Dmax=hSampSize; _Dmax.value=Dmax;}\n\ 
  Rem=_Rem.value;\n\
  set.style.visibility='hidden';\n\
};\n\
function stopRun(){\n\
  if (Run) {Run=false; _stopRun.value='Run';}\n\
  else {Run=true; _stopRun.value='Stop';};\n\
};\n\
function drawComm(){\n\
  gr.fillText(Rem,commX,commY);\n\
  gr.fillText('D='+Distance.toFixed(0),commX,commY+40);\n\
  gr.fillText('FFT='+Dmin+'..'+Dmax,commX,commY+80);\n\
  gr.fillText('AdB='+Amin+'..'+Amax,commX,commY+120);\n\
  gr.fillText('dTick='+dTick,commX,commY+160);\n\
  gr.fillText(configStr,commX,commY+200);\n\
}//drawComm\n\
function SavePNG(){\n\
  gr.fillStyle='#ffffff'; gr.font='bold 30px serif';\n\
  drawComm();\n\
  fName='M'+DateToStr()+'.png';\n\	  
  imageDataUrl=pic.toDataURL('image/png');\n\  
  alink=document.createElement('a');\n\
  alink.download=fName;\n\ 
  alink.href=imageDataUrl;\n\ 
  alink.click();\n\
};\n\
function DateToStr(){ var Dt=new Date();\n\
  yy=Dt.getFullYear(); mn=decToStr2(Dt.getMonth()+1); dd=decToStr2(Dt.getDate());\n\ 
  var s=''+yy+mn+dd; hh=decToStr2(Dt.getHours()); mm=decToStr2(Dt.getMinutes());\n\ 
  s+=hh+mm; return s;\n\
}//DateToStr\n\
function decToStr2(v) {return v.toString(10).padStart(2,'0')}\n\
</script>\n\
</head><body>\n\
<div aligh=center> <canvas id=pic height=1042 width=662></canvas></td></tr>\n\
</div>\n\
<div id=nav style='position:absolute;left:340px; top:0px; width:400px'>\n\
<input type=text id=dist size=4 style='font-size:60px;text-align:right' value='#####.##'><br>\n\  
<input type=button onClick=showSet() value=Set></input> &nbsp; &nbsp;\n\
<input type=button id=_stopRun onClick=stopRun() value=Stop></input> &nbsp; &nbsp;\n\
<input type=button  onClick=SavePNG() value=Save></input>\n\ 
</div>\n\
<div id=set class=menu>\n\
<table align=center cellpadding=5px><tr><td colspan=5><b>Settings</b></td></tr>\n\
<td>Dmin=</td><td><input type=text id=_Dmin size=2 value=0></td><td width=40px></td>\n\
<td>Dmax=</td><td><input type=text id=_Dmax size=2 value=512></td></tr>\n\
<td>Amin=</td><td><input type=text id=_Amin size=2 value=0></td><td></td>\n\
<td>Amax=</td><td><input type=text id=_Amax size=2 value=110></td></tr>\n\
<tr><td>dTick=</td><td><input type=text id=_dTick size=2 value=2></td>\n\
<td></td><td colspan=2><input type=checkbox id=_Grey>Gray</input></td></tr>\n\
<tr><td colspan=5>Rem:<input type=text id=_Rem size=6</td></tr>\n\
<tr><td></td><td></td><td></td><td></td><td></td></tr>\n\
<td colspan=2><input type=button onclick=restoreSet() value=Restore></td><td></td>\n\
<td colspan=2><input type=button onclick=hideSet() value=Store></td></tr>\n\
</table>\n\
</div>\n\
<script>\n\
winH=window.innerHeight; winW=window.innerWidth;\n\
mobile=true; if (winH<winW) mobile=false;\n\
if(mobile) {picH=winH-100; picW=winW-40;\n\
  Draw=DrawP;\n\
  nav.style='position:absolute;top:'+(picH-120)+';left:'+(picW-320);\n\
  commX=picW-280; commY=picH-210;\n\
} else {picH=winH-20; picW=winW-40;\n\
  Draw=DrawL;\n\
  nav.style='position:absolute; top:20px; left:'+(picW-320);\n\
  commX=picW-260; commY=25;\n\
}\n\pic.height=picH; pic.width=picW;\n\
gr=pic.getContext('2d',{willReadFrequently:true});\n\ 
Ymin=0; Ymax=picH; Xmin=0; Xmax=picW; //pixel\n\
Dmin=0; Dmax=512; dD=20; dDs=100;//Discrete FFT\n\
Amin=0; Amax=120; dA=10; dAs=50; //A,db FFT\n\
Grey=false;\n\
dTick=2;\n\
fillBg();\n\
</script>\n\
</body>");
}//mapHTML