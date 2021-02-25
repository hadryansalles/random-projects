----------------------------------------------------------------
-- file: mux8x16.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 08/19/2020
-- brief: mux with 8 data_oututs of 16 bits each
----------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity mux8x16_tb is
end entity;

architecture a_mux8x16_tb of mux8x16_tb is
    -- mux8x16 interface
    component mux8x16
        port (
            sel : in unsigned(2 downto 0);
            in0 : in unsigned(15 downto 0);
            in1 : in unsigned(15 downto 0);
            in2 : in unsigned(15 downto 0);
            in3 : in unsigned(15 downto 0);
            in4 : in unsigned(15 downto 0);
            in5 : in unsigned(15 downto 0);
            in6 : in unsigned(15 downto 0);
            in7 : in unsigned(15 downto 0);
            data_out : OUT unsigned(15 downto 0)
        );
    end component;
    signal sel : unsigned(2 downto 0) := "000";
    signal in0, in1, in2, in3, in4, in5, in6, in7 : unsigned(15 downto 0) := "0000000000000000";
    signal data_out : unsigned(15 downto 0) := "0000000000000000";
begin
    uut: mux8x16 port map (
        sel => sel,
        in0 => in0,
        in1 => in1,
        in2 => in2,
        in3 => in3,
        in4 => in4,
        in5 => in5,
        in6 => in6,
        in7 => in7,
        data_out => data_out
    );
    process 
    begin
        sel <= "000";
        in0 <= "0000000000000001"; 
        in1 <= "0000000000000011"; 
        in2 <= "0000000000000111"; 
        in3 <= "0000000000001111"; 
        in4 <= "0000000000011111"; 
        in5 <= "0000000000111111"; 
        in6 <= "0000000001111111"; 
        in7 <= "0000000011111111"; 
        wait for 50 ns; 
        sel <= "001";
        wait for 50 ns; 
        sel <= "010";
        wait for 50 ns; 
        sel <= "011";
        wait for 50 ns; 
        sel <= "100";
        wait for 50 ns; 
        sel <= "101";
        wait for 50 ns; 
        sel <= "110";
        wait for 50 ns; 
        sel <= "111";
        wait for 50 ns;
        wait;
    end process;
end architecture;
