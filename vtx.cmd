# test and demo some of the stuff VTX can do
init
# set the loop flag
loop
# and off we go
fg white+
bg blue
cls
echo this is a test
crlf
echo+lo lower:this is a test of +LO
crlf
echo+up upper:this is a test of +UP
crlf
echo+se sentance:this is a test of +SE
crlf
echo+ca capital:this is a test of +CA
crlf 
crlf
echo This is a test. Please don't panic!
crlf
crlf
#
reset
bg blue
bold
echo+se bold:this is a test of BOLD
crlf
crlf
#
reset
bg blue
italic
echo+se italic:this is a test of ITALIC
crlf
crlf
#
reset
bg blue
underscore
echo+se underscope:this is a test of UNDERSCORE
crlf
crlf
#
reset
bg blue
reverse
echo+se reverse:this is a test of REVERSE
crlf
crlf
#
reset
bg blue
blink
echo+se blink:this is a test of BLINK
crlf
crlf
sleep 100
# look at some colored text
init
fg white+
bg blue
cls
echo this is a test of white+ on blue
crlf
fg white
echo this is a test of white  on blue
crlf
#
fg red+
echo this is a test of red+ on blue
crlf
fg red
echo this is a test of red  on blue
crlf
#
fg green+
echo this is a test of green+ on blue
crlf
fg green
echo this is a test of green  on blue 
crlf
#
fg yellow+
echo this is a test of yellow+ on blue
crlf
fg yellow
echo this is a test of yellow  on blue
crlf
#
fg magenta+
echo this is a test of magenta+ on blue
crlf
fg magenta
echo this is a test of magenta  on blue
crlf
#
fg cyan+
echo this is a test of cyan+ on blue
crlf
fg cyan
echo this is a test of cyan  on blue
crlf
#
fg black+
echo this is a test of black+ on blue
crlf
fg black
echo this is a test of black  on blue
crlf
# Demo for High Nibble
sleep 100
init
bg blue
cls
fg white+
ban       The High Nibble Presents....
crlf
#
fg green+
ech                       The VT132 for the RC2014 platform
crlf
#
fg yellow+
ech                     The S-132 for the IMSAI 8080 Emulator
crlf
#
fg cyan+
ech                     The Standalone VT132 for other systems
crlf
crlf
crlf
crlf
crlf
fg yellow+
ech                         For the more significant bits
sleep 2
fg green++
docr
ech                         For the more significant bits
sleep 2
fg yellow+
docr
ech                         For the more significant bits
sleep 2
fg green++
docr
ech                         For the more significant bits
sleep 2
fg yellow+
docr
ech                         For the more significant bits
sleep 2
fg green++
docr
ech                         For the more significant bits
sleep 2
fg yellow+
docr
ech                         For the more significant bits
sleep 2
fg green++
docr
ech                         For the more significant bits
sleep 2
fg yellow+
docr
ech                         For the more significant bits
sleep 2
fg green++
docr
ech                         For the more significant bits
sleep 2
fg yellow+
docr
ech                         For the more significant bits
sleep 2
fg green++
docr
ech                         For the more significant bits
sleep 2
crlf
crlf
crlf
crlf
crlf
crlf
crlf
crlf
fg black
bold
ech                       https://thehighnibble.com/imsai8080
crlf
ech                         https://thehighnibble.com/vt132
crlf
ech                         https://thehighnibble.com/s132
crlf
ech                       https://thehighnibble.com/cromemcoZ1
sleep 100
cls
reset
bg blue
setp 1,1
fg green+
echo http://www.youtube.com/shadowtronblog
setp 5,5
fg white+
echo http://www.youtube.com/shadowtronblog
setp 10,10
fg cyan+
echo http://www.youtube.com/shadowtronblog
setp 15,15
fg yellow+
echo http://www.youtube.com/shadowtronblog
setp 20,20
fg red+
echo http://www.youtube.com/shadowtronblog
sleep 100

