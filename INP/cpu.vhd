-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2021 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Kristof Siska, xsiska16
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- ram[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_WREN  : out std_logic;                    -- cteni z pameti (DATA_WREN='0') / zapis do pameti (DATA_WREN='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WREN musi byt '0'
   OUT_WREN : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is


  --- PC
    signal pc_data : std_logic_vector (11 downto 0);
    signal pc_inc : std_logic;
    signal pc_dec : std_logic;
  --- PC
  
	
  --- CNT 
	signal counter : std_logic_vector(9 downto 0) := (others => '0');
  --- CNT

  --- PTR
    signal ptr_data : std_logic_vector (9 downto 0);
    signal ptr_inc : std_logic;
    signal ptr_dec : std_logic;

  --- FSM
  type FSM is (
    default_state,

    fetch_state,
    decode_state,
    execute_state,

    incr_ptr_state, 
    decr_ptr_state,

    incr_actual_state, 
    incr_actual_state_2,
    incr_actual_state_3,
    
    decr_actual_state,
    decr_actual_state_2,
    decr_actual_state_3,
    

    while_start_state,
    while_start_state_2,

    while_stop_state,
    while_stop_state_2,

	 wait_for_loop_end_state,
	 wait_for_loop_end_state_2,
   wait_for_loop_end_state_3,
   wait_for_loop_end_state_4,
   wait_for_loop_end_state_5,
   wait_for_loop_end_state_6,
	 

   while_find_start_state,
	 while_find_start_state_2,
   while_find_start_state_3,
   while_find_start_state_4,
   while_find_start_state_5,

    print_char_state, 
    print_char_state_2,
    load_to_actual_state,
    load_to_actual_state_2,

    break_loop_state, 
    end_program
  );
--- FSM

   
    signal mux_selector : std_logic_vector(1 downto 0) := (others => '0');
    signal mux_output : std_logic_vector(7 downto 0) := (others => '0');

    signal current_state : FSM := default_state;
    signal next_state : FSM := default_state;
begin

  --- PC ---
  pc : process(CLK, RESET, pc_inc, pc_dec) is
    begin
      if (RESET = '1') then

          pc_data <= (others => '0');
      elsif (rising_edge(CLK)) then 

          if (pc_inc = '1') then
            pc_data <= pc_data + 1;

          elsif (pc_dec = '1') then
            pc_data <= pc_data - 1;

          end if; --- rising_edge
      end if; --- reset
      
      CODE_ADDR <= pc_data;
    
    end process;
--- PC ---

--- PTR ---
  ptr : process(CLK, RESET, ptr_inc, ptr_dec ) is 

  begin 
    if (RESET = '1') then
      ptr_data <= (others => '0');

    elsif (rising_edge(CLK)) then

      if (ptr_inc = '1') then
        ptr_data <= ptr_data + 1;

      elsif (ptr_dec = '1') then
        ptr_data <= ptr_data - 1;

      end if; --- rising_edge
  end if; --- reset

  DATA_ADDR <= ptr_data;
  end process;
--- PTR ---

	OUT_DATA <= DATA_RDATA;

--- MUX ---
  mux : process(CLK, RESET, mux_selector) is 
  begin 
    if (RESET = '1') then
      mux_output <= (others => '0');
    elsif(rising_edge(CLK)) then 
       case mux_selector is 
           when "00" =>
                     mux_output <= IN_DATA;
           when "01" => 
                     mux_output <= DATA_RDATA + 1;

          when "10" =>
                     mux_output <= DATA_RDATA - 1;
          when others =>
                     mux_output <= (others => '0');
          end case;
    end if;
  end process;
  DATA_WDATA <= mux_output;

  --- MUX ---

--- FSM LOGIC --- 
  FSM_CURRENT_STATE : process(CLK, RESET, EN) is 
  begin 
	 if (RESET = '1') then
			current_state <= default_state;
    elsif (rising_edge(CLK)) then    
        if (EN = '1') then
				current_state <= next_state;
        end if;
    end if;  
  end process;

  --- FSM LOGIC

  FSM_NEXT_STATE : process (current_state, CODE_DATA, IN_VLD, DATA_RDATA, OUT_BUSY) is 
  begin
		
		
    --- INIT ---
        
        pc_inc <= '0';
        pc_dec <= '0';

        
        ptr_inc <= '0';
        ptr_dec <= '0';

        CODE_EN <= '0';
        DATA_EN <= '0';
        IN_REQ <= '0';
		  OUT_WREN <= '0';
    
    --- INIT ---
      case current_state is 
      
      when default_state => 
                      next_state <= fetch_state;  
      when fetch_state => 
                      CODE_EN <= '1';
                      next_state <= decode_state;


      when decode_state =>
        case CODE_DATA is
          when x"3E" =>
                  next_state <= incr_ptr_state;	
                  
          when x"3C" =>
                  next_state <= decr_ptr_state;
          
          when x"2B" =>
                  next_state <= incr_actual_state;
          
          when x"2D" => 
                  next_state <= decr_actual_state;
						
			 when x"5B" =>
						next_state <= while_start_state;
					
			 when x"5D" =>
						next_state <= while_stop_state;
          
          when x"2E" =>
                  next_state <= print_char_state;
          
          when x"2C" =>
                  next_state <= load_to_actual_state;
        
          when x"7E" =>
                  next_state <= break_loop_state;
						
			 when x"00" =>
						next_state <= end_program;
         
          when others => pc_inc <= '1';
        end case;


          when incr_ptr_state =>
						pc_inc <= '1';
						ptr_inc <= '1';
                  next_state <= fetch_state;
          
          when decr_ptr_state =>
                  pc_inc <= '1';
                  ptr_dec <= '1';
                  next_state <= fetch_state;

          when incr_actual_state =>
                  DATA_EN <= '1';
                  DATA_WREN <= '0';
                  next_state <= incr_actual_state_2;

          
          when incr_actual_state_2 =>
                 mux_selector <= "01";  -- Set the mux to increase value of WDATA
                 next_state <= incr_actual_state_3;

          when incr_actual_state_3 =>
                 DATA_EN <= '1';
                 DATA_WREN <= '1';
                 pc_inc <= '1';
                 next_state <= fetch_state;

          
          when decr_actual_state =>
                 DATA_EN <= '1';
                 DATA_WREN <= '0';
                 next_state <= decr_actual_state_2;
          
          when decr_actual_state_2 =>
                mux_selector <= "10";  -- Set the mux to decrease value of WDATA
                next_state <= decr_actual_state_3;

          when decr_actual_state_3 =>
                DATA_EN <= '1';
                DATA_WREN <= '1';
                pc_inc <= '1';
                next_state <= fetch_state;
          

          when print_char_state =>
                DATA_EN <= '1';
                DATA_WREN <= '0';
                next_state <= print_char_state_2;

          when print_char_state_2 =>
                if OUT_BUSY = '1' then 
                        DATA_EN <= '1';
								DATA_WREN <= '0';
								next_state <= print_char_state_2;
                else 
                        OUT_WREN <= '1';
                        pc_inc <= '1';
                        next_state <= fetch_state;
                end if;
          

          when load_to_actual_state =>
                IN_REQ <= '1';
                mux_selector <= "00";  -- Set the mux selector to write IN_DATA to DATA_WDATA
                next_state <= load_to_actual_state_2;
          
          when load_to_actual_state_2 =>
                if IN_VLD = '1' then 
                    DATA_EN <= '1';
                    DATA_WREN <= '1';
                    pc_inc <= '1';
                    next_state <= fetch_state;
                else 
                    --- We didnt get an input, so we are still wating
                    IN_REQ <= '1';
                    mux_selector <= "00";
                    next_state <= load_to_actual_state_2;
                end if;
					 
			 when end_program =>
			 			counter <= "0000000000";
						null;

	--- WHILE ---
       -- Found '[' and we are starting loop 
			 when while_start_state =>
						DATA_EN <= '1';
						DATA_WREN <= '0';
						next_state <= while_start_state_2;

			when while_start_state_2 =>
					if (DATA_RDATA = "00000000") then     -- We are skipping this loop
                				counter <= "0000000001";
								next_state <= wait_for_loop_end_state;
						else 
								pc_inc <= '1';
								next_state <= fetch_state;
					end if;
			 
		
			 when wait_for_loop_end_state =>
            if (counter = "0000000000") then
                  			pc_inc <= '1';
                  			next_state <= fetch_state;
            else
							pc_inc <= '1';
							pc_dec <= '0';
      						next_state <= wait_for_loop_end_state_2;

            end if;
						
		

         when wait_for_loop_end_state_2 =>
              CODE_EN <= '1';
              next_state <= wait_for_loop_end_state_5;

			  when wait_for_loop_end_state_5 =>
			  next_state <= wait_for_loop_end_state_3;

			when wait_for_loop_end_state_3 =>
						if (CODE_DATA = x"5D") then -- ]
                  counter <= counter - 1;
                  next_state <= wait_for_loop_end_state_4;
										
						elsif (CODE_DATA = x"5B") then   --- [ 
                  counter <= counter + 1;
									next_state <= wait_for_loop_end_state_4;

            else 
                  next_state <= wait_for_loop_end_state_4;
							
						end if;

      when wait_for_loop_end_state_4 =>
                  next_state <= wait_for_loop_end_state;



      	 -- Found ']' and we are repeating or stopping looping
			 when while_stop_state =>
       DATA_EN <= '1';
       DATA_WREN <= '0';
       next_state <= while_stop_state_2;

  when while_stop_state_2 =>
       if (DATA_RDATA = "00000000") then  -- We are stopping this loop
           pc_inc <= '1';
           next_state <= fetch_state;
       else 
           counter <= "0000000001";               -- We are not stopping this loop
           pc_dec <= '1';
           next_state <= while_find_start_state;
       
       end if;
--				
						
      
      when while_find_start_state =>
              if (counter = "0000000000") then
                  pc_inc <= '1';
                  next_state <= fetch_state;
              else 
				  pc_dec <= '1';
				  pc_inc <= '0';
                  next_state <= while_find_start_state_2;
              end if;
					
      

      when while_find_start_state_2 =>
                CODE_EN <= '1';
                next_state <= while_find_start_state_5;

				when while_find_start_state_5 =>
				next_state <= while_find_start_state_3;
	
      
			when while_find_start_state_3 =>
            case CODE_DATA is
               when x"5B" =>
                    counter <= counter - 1;
                    next_state <= while_find_start_state_4;

                when x"5D" =>
                    counter <= counter + 1;
                    next_state <= while_find_start_state_4;
                    
                when others => 
                    next_state <=  while_find_start_state_4;
						
					end case;
								
			when while_find_start_state_4 =>
              if (counter = "0000000000") then
                pc_inc <= '1';
                next_state <= fetch_state;
              else
                pc_dec <= '1';
                next_state <= while_find_start_state_2;
              end if;	       

			when others => pc_inc <= '1';
            
      end case;
    	
  end process;



  end behavioral;
 
