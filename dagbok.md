# Passap E6000


# 2/5/24

Samskipti helgu við Chris ofthestreet og
Annacroucher
## TODO, Hvað er?
* WinCrea
* KnitterStream
* DIN port
* Hvaða útgáfu erum við með?

<!-- `Creating our own circuit board, since arduino is too bulky and attaching that into the console` -->

### DIN port
Í tölvupóstasamskiptum er mynd þar sem sýnt er 6 pin DIN port.
<img src="./img/oQIQN.png" style="background-color: white;" alt="Mynd af din 6 pin connector tengjum"/>


### WinCrea
forrit til að gera hannanir fyrir Passap e6000 outputar .cut fileum sem er síðan hægt að h
### KnitterStream
* Notast við arduino til að interface við takka á stýringu.
* Hleður inn skrám eins og hægt er í chipset 23+ 

### Hvaða útgáfu erum við með?

Croucher vill að maður viti hvaða chipset vélin hefur en lýsingar til að komast að því eru óskýrar og krefjast jafnvel þess að rífa hana í sundur sem ég hef ekki leyfi fyrir enn.

## Chipset test
[Diana Natters umfjöllun um passap e6000](https://diananatters.blogspot.com/2010/10/passap-e6000-battery-chipset.html)


If it says MEMO, then start at step 1.

Þegar vélin er ræst kemur upp MEMO

Vélin öskrar og við fáum error 213.

Hún getur því ekki tekið við niðurhöluðum skrám.

## Croucher

Við erum með id 7041398 > 7037001.
Skv skjölum frá croucher eigum við því að vera með download og 32kb minni. En testið sem ég gerði frá diönu gaf error 213. Las yfir documentation fræa croucher aftur og hann seigir að error 213 sé rétt. Rámaði í það að ég hefði séð þessa tölu áður. Diana skrifar óskýrt en þetta er þá komið núna. Við erum með stuðning við að downloada patternum.

Það eru til mismunandi týpur af vélinni.
Það var bætt við download support frá MSDOS og auka minni í nýrri vélunum.
Það er hægt að uppfæra í það og kaupa frá honum íhluti til þess. Hann mæli eindregi með því að maður leggji ekki í það að uppfæra minnið og virðast ekki vera miklir kostir til þess.

Hægt er að hlaða niður skjölum, .cut sem innihalda
leiðbeiningar um lita munstur upp að 4 litum.
0,1,2,3
Leiðbeiningar um uppsetningar slíkar skrár eru í skjölum sem croucher sendi.


Það skjal inniheldur header með 10 stöfum og svo munstrið.

* Litafjölda stafur [0,3,4]: HEX[30,33,34] = [1-2,3,4] Litir
* Bil: HEX[20]
* Breidd munsturs/dálkar á bilinu [bil,bil,1] til [1,8,0]
* Bil
* Lengd munsturs/raðir á bilinu [bil,bil,1] til [2,5,5]
* Bil
* Litamunstur þar sem hver tala merkir lit 0-3

Hef það á tilfinningunni að við séum með eldri týpu annars væri ég ekki að lesa tölvupósta frá 2014.

## Irene
[Irene](https://hackaday.io/project/163701-passap-e6000-rebuilt-and-replaced-console/details)
[Irene repo](https://github.com/IrenePassap/Passap-E6000-hacked-and-rebuilt/blob/master/Passap_Projekt_Raspberry_Pi_Version_36.3_23.7.21.py)

Hún notar eina raspberry pi sem stýrir svo þrem arduino vélum. ein fyrir motor til að færa fram og til baka, ein fyrir fremri 'lock' til að prjóna og aðra fyrir aftari 'lock'.

Hún er búin að breyta vélinni þannig að aftara borðið er eins og fremra og getur þá líka prjónað eh meira fancy.

Hún vitnar í [bamberg](https://www.hackerspace-bamberg.de/Passap_pfaff_e6000)
.

Bamberg talar 'lightbarriers' í "carriage" til að vita staðsetninguna.

Finnst líklegt að við þurfum bara einfaldlega að losta okkur við upprunalgeu stýringuna eins og aðrir hafa gert. Þá kemur upp vandamál Croucher um að þú þarft í raun að forrita allt frá grunni með mikilli vitneskju um prjón og vélbúnaðinn.


## Niðurstöður // Summary

Erum með stuðning við að downloada .cut fileum.

Höfum 32k chip en 20k minni skv nýtni áætlun croucher.

Irene og bamberg hafa fjarlægt consolið gjörsamlega og smíðað sitt eigið með raspberry pi.

Ég held að við viljum halda consolinu og allri tækninni þar inni. grf.

Spurning um að bæta þá ofaná eh stýringu sem interfacer við consolið. Skoða knitterstream betur.




# 3/5/24

Byrjum á að skoða Irene og Bamberg betur.

* [Bamberg Github](https://github.com/knitty/firmware/tree/master/software/Knitty)
* [Bamberg lýsing (þýsk)](https://www.hackerspace-bamberg.de/Passap_pfaff_e6000)
* [Irene Github](https://github.com/IrenePassap/Passap-E6000-hacked-and-rebuilt)
* [Irene dagbók](https://hackaday.io/project/163701-passap-e6000-rebuilt-and-replaced-console/details)


## Diagram
### Bamberg

![BambergDiargram](./img/BambergDiagram.png)
Arduino unoinn kemur í stað stýringarinar 'consolsins' og notast þeir við sitt eigið forrit sem þeir gerðu fyrir alíka vél sem heitir [knitty](https://www.hackerspace-bamberg.de/Knitty)

Þeir eru líka með sér arduino fyror Autocolor, hvort hann hafi verið eh bilaður hjá þeim. Sé þetta ekki hjá Irene

<img src="./img/BambergDiagramColor.png" style="background-color: white;" alt="mynd af tengingum við servo sem stýra litaskiptingum"/>

[`Hægt er að sjá .svg diagram myndir frá Bamberg`](https://github.com/knitty/firmware/tree/master/hardware)

### Irene
![IreneDiagram](./img/IreneDiagram.png)
Irene byggir á Bamberg en í stað þess að vera með tölvu usb tengda í arduino uno þá er raspberry pi með notendaviðmót sem stýrir

Lesa kóðan þeirra og skilja hvernig þau eru að lesa inn staðsetningu með pinum frá ljósunum. Þetta virðist ekki flókið en erfitt að ná hausnum almennilega utanum þetta án  þess að fikta sjálfur.
![Mynd af seglum frá itene](https://cdn.hackaday.io/images/5890161549120496890.png)
<a href="https://hackaday.io/project/163701-passap-e6000-rebuilt-and-replaced-console/details" style="text-align:center;">Sjá meira hjá Irene</a>
Light sensorinn skýnir ljósi í 2mm göt á guiderail, 3mm á milli gata, 5mm er ein nál.

út frá munstri er hægt að lesa staðsetningu og átt lásins.

Ég bætti við Repoinu hennar Irene og bamber hér í möppu, til að auðvelda mér að fina efnið og rýna í það.