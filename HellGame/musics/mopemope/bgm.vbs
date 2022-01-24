Dim music
Set music = CreateObject("WMPlayer.OCX")
music.URL = "musics/mopemope/bgm.mp3"
Do Until music.playState = 1
	WScript.Sleep 1000
Loop
