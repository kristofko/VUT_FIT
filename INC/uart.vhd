-- uart.vhd: UART controller - receiving part
-- Author(s): Kristof Siska - xsiska16
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

-------------------------------------------------
entity UART_RX is
port(	
    	CLK      :   	in std_logic;
	RST      : 	in std_logic;
	DIN      : 	in std_logic;
	DOUT     : 	out std_logic_vector(7 downto 0);
	DOUT_VLD : 	out std_logic
);
end UART_RX;  

-------------------------------------------------
architecture behavioral of UART_RX is
signal cnt_clk     : std_logic_vector(4 downto 0);
signal cnt_bit     : std_logic_vector(3 downto 0);
signal rec_en      : std_logic;
signal cnt_en      : std_logic;
signal data_valid  : std_logic;

begin
	-- FSM ports 
	FSM: entity work.UART_FSM(behavioral)
	port map (
		CLK => CLK,
		RST => RST,
		DIN => DIN,
		CNT_CLK => cnt_clk,
		CNT_BIT => cnt_bit,
		REC_EN => rec_en,
		CNT_EN => cnt_en,
		DATA_VALID => data_valid	
	);
	
	process (CLK) begin
	if rising_edge(CLK) then
		DOUT_VLD <= data_valid;
		if RST = '1' then
			cnt_clk <= "00000";
			cnt_bit <= "0000";
		end if;
		
		if cnt_en = '1' then
			cnt_clk <= cnt_clk + 1;
		else
			cnt_clk <= "00000";  -- Null the counter for consistency
		end if;
		
		if data_valid = '1' then
			cnt_clk <= "00001";
		end if;
		if rec_en = '0' then
			cnt_bit <= "0000"; -- Null the counter for consistency
		end if;

			if rec_en = '1' then
				if cnt_clk = "01111" or cnt_clk = "11000" then
					cnt_clk <= "00000";
					case cnt_bit is 
						when "0000" => DOUT(0) <= DIN;
						when "0001" => DOUT(1) <= DIN;
						when "0010" => DOUT(2) <= DIN;
						when "0011" => DOUT(3) <= DIN;
						when "0100" => DOUT(4) <= DIN;
						when "0101" => DOUT(5) <= DIN;
						when "0110" => DOUT(6) <= DIN;
						when "0111" => DOUT(7) <= DIN; 
						when others => null;
					end case;
					cnt_bit <= cnt_bit + 1;
				end if;
			end if;	
		end if; -- rising_edge
	end process;	
end behavioral;
