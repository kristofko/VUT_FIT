; Vernamova sifra na architekture DLX
; jmeno prijmeni login

        .data 0x04          ; zacatek data segmentu v pameti
login:  .asciiz "xsiska16"  ; <-- nahradte vasim loginem
cipher: .space 9 ; sem ukladejte sifrovane znaky (za posledni nezapomente dat 0)

        .align 2            ; dale zarovnavej na ctverice (2^2) bajtu
laddr:  .word login         ; 4B adresa vstupniho textu (pro vypis)
caddr:  .word cipher        ; 4B adresa sifrovaneho retezce (pro vypis)

        .text 0x40          ; adresa zacatku programu v pameti
        .global main        ; 
			; xsiska16-r4-r17-r27-r28-r29-r0
			; add into byte, s = 19
			; subtract from byte i = 9
			; ascii value of z = 122
			; ascii value of a = 97
main:   

	addi r4, r0 ,0
	addi r27, r0 ,0 
	; r4 - Char counter
	; r17 - login memory place
 	; r27 - Reg for byte operations
	; r28,r29 Reg for arithmetic and logic operations

	; ak je r4 0 tak pridavame do bajtu
	; ak je r4 != 0 tak odoberame
	j repeat

repeat:
	
	lb r27, login(r17)
	; --- Check if character is less than 97  ---
	addi r28 , r0 , 97
				;        (nacitany znak > 96)	
	sgt r29, r28, r27    ; r29 = 1 ak r27>r28 0 otherwise 
	
	bnez r29, end
	nop
	; --- End of character check ---
	
	bnez r4, subtract_from_byte
	nop
	addi r4, r0, 1  ; Set the r4 to 1, indicating that next byte will be subtracted from
	addi r27, r27, 19
	addi r28, r0, 122
	sgt r29, r27, r28 		; if byte + s >= 122 => byte = byte - 26
	bnez r29, subtract_26
	nop
	nop
	j store_byte
	nop


subtract_26:
	subi r27, r27, 26
	nop
	j store_byte
	nop


subtract_from_byte:
	subi r4, r4, 1
	subi r27, r27, 9
	addi r28, r0, 97
	sgt r29, r28, r27 	; if byte - i <= 97 => byte = byte + 26
	bnez r29, add_26
	nop
	j store_byte
	nop
	


add_26:
	addi r27, r27, 26
	j store_byte
	nop
	
store_byte:
	nop
	sb cipher(r17), r27
	nop
	addi r17, r17, 1
	j repeat
	nop
	

end:    
	sb cipher(r17), r0  ; Get null byte to the end of the ciphered word
	addi r14, r0, caddr ; <-- pro vypis sifry nahradte laddr adresou caddr
        trap 5  ; vypis textoveho retezce (jeho adresa se ocekava v r14)
        trap 0  ; ukonceni simulace
