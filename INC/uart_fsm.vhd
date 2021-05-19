-- uart_fsm.vhd: UART controller - finite state machine
-- Author(s): Kristof Siska - xsiska16
--
library ieee;
use ieee.std_logic_1164.all;

-------------------------------------------------
entity UART_FSM is
port(
   CLK   		: in std_logic;
   RST   		: in std_logic;
   DIN  		: in std_logic;
   CNT_CLK   		: in std_logic_vector(4 downto 0);
   CNT_BIT  	        : in std_logic_vector(3 downto 0);
   REC_EN 	        : out std_logic;
   CNT_EN  		: out std_logic;
   DATA_VALID           : out std_logic
   );
end entity UART_FSM;

-------------------------------------------------
architecture behavioral of UART_FSM is
type STATES is ( START_BIT , WAIT_FIRST_BIT, RECEIVE_BYTE, STOP_BIT, VALIDATE_DATA );
signal current_state : STATES := START_BIT;
begin
	CNT_EN <= '1' when current_state = RECEIVE_BYTE or current_state = WAIT_FIRST_BIT else '0'; -- Enable clock_counter
	REC_EN <= '1' when current_state = RECEIVE_BYTE else '0';	-- Enable mapping DIN to the right DOUT position				
	DATA_VALID <= '1' when current_state = VALIDATE_DATA else '0';  -- Validate data for 1 clock cycle
process (CLK) begin
	if rising_edge (CLK) then
	    if RST = '1' then
	        current_state <= START_BIT;
		
	    else 
			case current_state is
			when START_BIT   =>    if DIN = '0' then
											current_state <= WAIT_FIRST_BIT;
										end if;
			
			when WAIT_FIRST_BIT   =>    if CNT_CLK = "10111" then
											current_state <= RECEIVE_BYTE;
										end if;
						
			when RECEIVE_BYTE      =>    if CNT_BIT = "1000" then
											current_state <= STOP_BIT;
										end if;

			when STOP_BIT    =>    if DIN = '1' then
											current_state <= VALIDATE_DATA;
										end if;
									
			when VALIDATE_DATA   =>  	current_state <= START_BIT;
											
											
			when others 	      => null;		    
	    	end case;
		end if;
	end if;
end process;    

end behavioral;
