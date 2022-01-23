import numpy as np
import matplotlib.pyplot as plt
from numpy.lib.function_base import append
from scipy.signal.spectral import spectrogram
import soundfile as sf
import scipy.signal
import cmath

def function_dft(ramec):
	t = np.zeros(1024)
	N = len(ramec)
	for k in range(N):
		num = 0
		for n in range(N):
			num += ramec[n]*cmath.exp(-2j*cmath.pi*k*n*(1/N))
		t[k] = abs(num)
		
	return t

def find_peaks(ramec):
	t = np.zeros(100)
	N = len(ramec)
	prev_val = 0
	curr_val = 0
	n_val = 0
	index = int(0)
	for i in range(N):

		if (curr_val > prev_val and curr_val < n_val):
			t[index] = curr_val


znely_ramec = 45


s, fs = sf.read('xsiska16.wav')

print("Pocet vzorkov: ", s.size)

max = max(s)
min = min(s)

print("max ", max)
print("min ", min)


t = np.arange(s.size) / fs

time = s.size / fs

print("Cas nahravky v sekundach: ", time)


plt.figure(figsize=(6,3))
plt.plot(t, s)

plt.gca().set_xlabel('$t[s]$')
plt.gca().set_title('Signal')
plt.tight_layout()

plt.savefig('Uloha1.png')

mean = np.mean(s)

min = 0 - min 

if min > max :
	for i in range(s.size):
			s[i] = s[i] - mean 
			s[i] = s[i] / min 

else :
	for i in range(s.size):
			s[i] = s[i] - mean
			s[i] = s[i] / max


pocet_ramcov = s.size // 512

pocet_ramcov += pocet_ramcov

current_ramec = 0

x = 1024

y = 512


ramce = [[0 for i in range(pocet_ramcov)] for i in range(x)] 



for i in range(pocet_ramcov):
	ramce[i] = s[i * 512: i * 512 + 1024]




# Vytvorenie grafu zneleho ramnca
t = np.arange(ramce[znely_ramec].size) / fs

plt.figure(figsize=(8,4))
plt.plot(t, ramce[znely_ramec])

plt.gca().set_xlabel('$t[s]$')
plt.gca().set_title('Znely ramec')
plt.tight_layout()
plt.savefig('Uloha2.png')


# Chceme analyzovat frek. char. ramce[30]

fft =  np.fft.fft(ramce[znely_ramec]) 

t = np.arange(fft.size) / (1024/fs)

plt.figure(figsize=(6,3))

plt.plot(t[:t.size//2+1], np.abs(fft[:fft.size//2+1]))

plt.gca().set_xlabel('$Fs[Hz]$')
plt.gca().set_title('DFT znely ramec - numpy')

plt.tight_layout()

plt.savefig('Uloha3.1.png')


# POUZITIE MOJEJ DFT FUNKCIE NAD JEDNYM RAMCOM

# fft = function_dft(ramce[znely_ramec])
# t = np.arange(fft.size) / (1024/fs)
# plt.figure(figsize=(6,3))

# plt.plot(t[:t.size//2+1], np.abs(fft[:fft.size//2+1]))
# plt.gca().set_xlabel('$Fs[Hz]$')
# plt.gca().set_title('DFT znely ramec - moje')
# plt.tight_layout()
# plt.savefig('Uloha3.2.png')

# KONIEC MOJEJ DFT FUNKCIE

peaks = scipy.signal.find_peaks(np.abs(fft))

peak_x_values = np.zeros(4)

index = int(0)

for i in range(peaks[0].size //2+1):
	if (np.abs(fft[peaks[0][i]]) > 10):
		peak_x_values[index] = int(peaks[0][i])
		index = index + 1
	
for i in range(4):
	print(int(peak_x_values[i]))

print("Frequencies of \"rusive elementy\"")

multiplier = 16000/1024

for i in range(4):
	print(peak_x_values[i] * multiplier)



# Prevzate od https://nbviewer.org/github/zmolikova/ISS_project_study_phase/blob/master/Zvuk_spektra_filtrace.ipynb
f, t, sgr = spectrogram(s, fs)
# prevod na PSD
# (ve spektrogramu se obcas objevuji nuly, ktere se nelibi logaritmu, proto +1e-20)
sgr_log = 10 * np.log10(sgr+1e-20) 
plt.figure(figsize=(9,3))
plt.pcolormesh(t,f,sgr_log)
plt.gca().set_title('Spektrogram signalu')
plt.gca().set_xlabel('Čas [s]')
plt.gca().set_ylabel('Frekvence [Hz]')
cbar = plt.colorbar()
cbar.set_label('Spektralní hustota výkonu [dB]', rotation=270, labelpad=15)

plt.tight_layout()
plt.savefig('Uloha4.png')


freq1 = 671.875
freq2 = 1328.125
freq3 = 2000.0
freq4 = 2656.25

fft = np.abs(np.fft.fft(ramce[0])) 

t = np.arange(fft.size) / (1024/fs)

# Plotting filtered fft
plt.figure(figsize=(6,3))
plt.plot(t[:t.size//2+1], fft[:fft.size//2+1])
plt.gca().set_xlabel('$Hz$')
plt.gca().set_title('Signal')
plt.tight_layout()

plt.savefig('Uloha5.1.png')


sr = fs
# sampling interval
ts = time/sr
t = np.arange(s.size) / fs

# frequency of the signal
freq = freq1
signal1 = np.cos(2*np.pi*freq*t)
freq = freq2
signal2 = np.cos(2*np.pi*freq*t)
freq = freq3
signal3 = np.cos(2*np.pi*freq*t)
freq = freq4
signal4 = np.cos(2*np.pi*freq*t)

signal_out = signal1 + signal2 + signal3 + signal4

min = np.min(signal_out)
max = np.max(signal_out)

min = 0 - min

mean = np.mean(signal_out)

if min > max :
	for i in range(s.size):
			signal_out[i] = signal_out[i] - mean 
			signal_out[i] = signal_out[i] / min
			

else :
	for i in range(s.size):
			signal_out[i] = signal_out[i] - mean
			signal_out[i] = signal_out[i] / max

sf.write('audio/4cos.wav', signal_out, fs )

			

# Prevzate od https://nbviewer.org/github/zmolikova/ISS_project_study_phase/blob/master/Zvuk_spektra_filtrace.ipynb
f, t, sgr = spectrogram(signal_out, fs)
# prevod na PSD
# (ve spektrogramu se obcas objevuji nuly, ktere se nelibi logaritmu, proto +1e-20)
sgr_log = 10 * np.log10(sgr+1e-20) 
plt.figure(figsize=(9,3))
plt.pcolormesh(t,f,sgr_log)
plt.gca().set_title('Spektrogram - 4 Cosinusovky')
plt.gca().set_xlabel('Čas [s]')
plt.gca().set_ylabel('Frekvence [Hz]')
cbar = plt.colorbar()
cbar.set_label('Spektralní hustota výkonu [dB]', rotation=270, labelpad=15)

plt.tight_layout()
plt.savefig('Uloha6.png')

#Filtrovanie

signal_filtered = s

# filter bandstop - reject 671 Hz
low = freq1 
tolerate1 = 80
tolerate2 = 30
b, a = scipy.signal.buttord([(low - tolerate1)/(fs/2), (low + tolerate1)/(fs/2)], [(low - tolerate2)/(fs/2), (low + tolerate2)/(fs/2)], 3, 40)

b1, a1 = scipy.signal.butter(b, a, 'bandstop')

N_imp = 32
imp = [1, *np.zeros(N_imp-1)] # jednotkovy impuls
h = scipy.signal.lfilter(b1, a1, imp)

# frekvencni charakteristika
w, H = scipy.signal.freqz(b1, a1)

# nuly, poly
z, p, k = scipy.signal.tf2zpk(b1, a1)

# stabilita
is_stable = (p.size == 0) or np.all(np.abs(p) < 1) 

if is_stable == True:
	print("Filter je stabilny")
else:
	print("Filter je nestabilny")

# filtrace
sfa = scipy.signal.lfilter(b1, a1, s)
f, t, sfgr = spectrogram(sfa, fs)
sfgr_log = 10 * np.log10(sfgr+1e-20)
plt.figure(figsize=(5,3))
plt.stem(np.arange(N_imp), h, basefmt=' ')
plt.gca().set_xlabel('$n$')
plt.gca().set_title('Impulzná odozva filtra 1 $h[n]$')

plt.grid(alpha=0.5, linestyle='--')

plt.tight_layout()
plt.savefig("Uloha7.1.png")

plt.figure(figsize=(4,3.5))

# jednotkova kruznice
ang = np.linspace(0, 2*np.pi,100)
plt.plot(np.cos(ang), np.sin(ang))

# nuly, poly
plt.scatter(np.real(z), np.imag(z), marker='o', facecolors='none', edgecolors='r', label='nuly')
plt.scatter(np.real(p), np.imag(p), marker='x', color='g', label='póly')

plt.gca().set_title('Nuly/Poly 1')
plt.gca().set_xlabel('Realna zložka $\mathbb{R}\{$z$\}$')
plt.gca().set_ylabel('Imaginarna zložka $\mathbb{I}\{$z$\}$')

plt.grid(alpha=0.5, linestyle='--')
plt.legend(loc='upper left')

plt.tight_layout()
plt.savefig("Uloha8.1.png")

_, ax = plt.subplots(1, 2, figsize=(9,3))

ax[0].plot(w / 2 / np.pi * fs, np.abs(H))
ax[0].set_xlabel('Frekvence [Hz]')
ax[0].set_title('Modul frekvenční charakteristiky $|H(e^{j\omega})|$')

ax[1].plot(w / 2 / np.pi * fs, np.angle(H))
ax[1].set_xlabel('Frekvence [Hz]')
ax[1].set_title('Argument frekvenční charakteristiky $\mathrm{arg}\ H(e^{j\omega})$')

for ax1 in ax:
    ax1.grid(alpha=0.5, linestyle='--')

plt.tight_layout()
plt.savefig("Uloha9.1.png")

signal_filtered = scipy.signal.lfilter(b1, a1, signal_filtered)

# filtered signal

low = freq2

b, a = scipy.signal.buttord([(low - tolerate1)/(fs/2), (low + tolerate1)/(fs/2)], [(low - tolerate2)/(fs/2), (low + tolerate2)/(fs/2)], 3, 40)

b1, a1 = scipy.signal.butter(b, a, 'bandstop')

signal_filtered = scipy.signal.lfilter(b1, a1, signal_filtered)

imp = [1, *np.zeros(N_imp-1)] # jednotkovy impuls
h = scipy.signal.lfilter(b1, a1, imp)
sfa = scipy.signal.lfilter(b1, a1, s)
f, t, sfgr = spectrogram(sfa, fs)
sfgr_log = 10 * np.log10(sfgr+1e-20)
plt.figure(figsize=(5,3))
plt.stem(np.arange(N_imp), h, basefmt=' ')
plt.gca().set_xlabel('$n$')
plt.gca().set_title('Impulzná odozva filtra 2 $h[n]$')

plt.grid(alpha=0.5, linestyle='--')

plt.tight_layout()
plt.savefig("Uloha7.2.png")

z, p, k = scipy.signal.tf2zpk(b1, a1)
plt.figure(figsize=(4,3.5))

# jednotkova kruznice
ang = np.linspace(0, 2*np.pi,100)
plt.plot(np.cos(ang), np.sin(ang))

# nuly, poly
plt.scatter(np.real(z), np.imag(z), marker='o', facecolors='none', edgecolors='r', label='nuly')
plt.scatter(np.real(p), np.imag(p), marker='x', color='g', label='póly')

plt.gca().set_title('Nuly/Poly 2')
plt.gca().set_xlabel('Realna zložka $\mathbb{R}\{$z$\}$')
plt.gca().set_ylabel('Imaginarna zložka $\mathbb{I}\{$z$\}$')

plt.grid(alpha=0.5, linestyle='--')
plt.legend(loc='upper left')

plt.tight_layout()
plt.savefig("Uloha8.2.png")


low = freq3 

b, a = scipy.signal.buttord([(low - tolerate1)/(fs/2), (low + tolerate1)/(fs/2)], [(low - tolerate2)/(fs/2), (low + tolerate2)/(fs/2)], 3, 40)

b1, a1 = scipy.signal.butter(b, a, 'bandstop')

imp = [1, *np.zeros(N_imp-1)] # jednotkovy impuls
h = scipy.signal.lfilter(b1, a1, imp)
sfa = scipy.signal.lfilter(b1, a1, s)
f, t, sfgr = spectrogram(sfa, fs)
sfgr_log = 10 * np.log10(sfgr+1e-20)
plt.figure(figsize=(5,3))
plt.stem(np.arange(N_imp), h, basefmt=' ')
plt.gca().set_xlabel('$n$')
plt.gca().set_title('Impulzná odozva filtra 3 $h[n]$')

plt.grid(alpha=0.5, linestyle='--')

plt.tight_layout()
plt.savefig("Uloha7.3.png")

z, p, k = scipy.signal.tf2zpk(b1, a1)
plt.figure(figsize=(4,3.5))

# jednotkova kruznice
ang = np.linspace(0, 2*np.pi,100)
plt.plot(np.cos(ang), np.sin(ang))

# nuly, poly
plt.scatter(np.real(z), np.imag(z), marker='o', facecolors='none', edgecolors='r', label='nuly')
plt.scatter(np.real(p), np.imag(p), marker='x', color='g', label='póly')

plt.gca().set_title('Nuly/Poly 3')
plt.gca().set_xlabel('Realna zložka $\mathbb{R}\{$z$\}$')
plt.gca().set_ylabel('Imaginarna zložka $\mathbb{I}\{$z$\}$')

plt.grid(alpha=0.5, linestyle='--')
plt.legend(loc='upper left')

plt.tight_layout()
plt.savefig("Uloha8.3.png")


signal_filtered = scipy.signal.lfilter(b1, a1, signal_filtered)

low = freq4 

b, a = scipy.signal.buttord([(low - tolerate1)/(fs/2), (low + tolerate1)/(fs/2)], [(low - tolerate2)/(fs/2), (low + tolerate2)/(fs/2)], 3, 40)

b1, a1 = scipy.signal.butter(b, a, 'bandstop')

signal_filtered = scipy.signal.lfilter(b1, a1, signal_filtered)

N_imp = 32
imp = [1, *np.zeros(N_imp-1)] # jednotkovy impuls
h = scipy.signal.lfilter(b1, a1, imp)

# frekvencni charakteristika
w, H = scipy.signal.freqz(b1, a1)

# nuly, poly
z, p, k = scipy.signal.tf2zpk(b1, a1)

# stabilita
is_stable = (p.size == 0) or np.all(np.abs(p) < 1) 

if is_stable == True:
	print("Filter je stabilny")
else:
	print("Filter je nestabilny")

# filtrace
sfa = scipy.signal.lfilter(b1, a1, s)
f, t, sfgr = spectrogram(sfa, fs)
sfgr_log = 10 * np.log10(sfgr+1e-20)
plt.figure(figsize=(5,3))
plt.stem(np.arange(N_imp), h, basefmt=' ')
plt.gca().set_xlabel('$n$')
plt.gca().set_title('Impulzná odozva filtra 4 $h[n]$')

plt.grid(alpha=0.5, linestyle='--')

plt.tight_layout()
plt.savefig("Uloha7.4.png")

plt.figure(figsize=(4,3.5))

# jednotkova kruznice
ang = np.linspace(0, 2*np.pi,100)
plt.plot(np.cos(ang), np.sin(ang))

# nuly, poly
plt.scatter(np.real(z), np.imag(z), marker='o', facecolors='none', edgecolors='r', label='nuly')
plt.scatter(np.real(p), np.imag(p), marker='x', color='g', label='póly')

plt.gca().set_title('Nuly/Poly 4')
plt.gca().set_xlabel('Realna zložka $\mathbb{R}\{$z$\}$')
plt.gca().set_ylabel('Imaginarna zložka $\mathbb{I}\{$z$\}$')

plt.grid(alpha=0.5, linestyle='--')
plt.legend(loc='upper left')

plt.tight_layout()
plt.savefig("Uloha8.4.png")

_, ax = plt.subplots(1, 2, figsize=(9,3))

ax[0].plot(w / 2 / np.pi * fs, np.abs(H))
ax[0].set_xlabel('Frekvence [Hz]')
ax[0].set_title('Modul frekvenční charakteristiky $|H(e^{j\omega})|$')

ax[1].plot(w / 2 / np.pi * fs, np.angle(H))
ax[1].set_xlabel('Frekvence [Hz]')
ax[1].set_title('Argument frekvenční charakteristiky $\mathrm{arg}\ H(e^{j\omega})$')

for ax1 in ax:
    ax1.grid(alpha=0.5, linestyle='--')

plt.tight_layout()
plt.savefig("Uloha9.4.png")


# Prevzate od https://nbviewer.org/github/zmolikova/ISS_project_study_phase/blob/master/Zvuk_spektra_filtrace.ipynb
f, t, sgr = spectrogram(signal_filtered, fs)
# prevod na PSD
# (ve spektrogramu se obcas objevuji nuly, ktere se nelibi logaritmu, proto +1e-20)
sgr_log = 10 * np.log10(sgr+1e-20) 
plt.figure(figsize=(9,3))
plt.pcolormesh(t,f,sgr_log)
plt.gca().set_title('Spektrogram filtrovaneho signalu')
plt.gca().set_xlabel('Čas [s]')
plt.gca().set_ylabel('Frekvence [Hz]')
cbar = plt.colorbar()
cbar.set_label('Spektralní hustota výkonu [dB]', rotation=270, labelpad=15)

plt.tight_layout()
plt.savefig('Uloha10.1.png')

sf.write('audio/clean_bandstop.wav', signal_filtered.astype(np.float32) , fs)





