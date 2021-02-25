---------------------------------------------------------------
-- file: rom_tb.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 09/02/2020
-- brief: ROM tests
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity rom_tb is
end entity;

architecture a_rom_tb of rom_tb is
    -- ROM interface
    component rom
        port (
            clk : in std_logic := '0';
            address : in unsigned(6 downto 0) := "0000000";
            data_out : out unsigned(15 downto 0) := B"0000_0000_0000_0000"
        ); 
    end component;
    -- signals
    signal clk : std_logic := '0';
    signal address : unsigned(6 downto 0) := "0000000";
    signal data : unsigned(15 downto 0) := B"0000_0000_0000_0000";
begin 
    -- connect to rom
    uut : rom port map(clk, address, data);
    -- clock pulse
    process
    begin
       clk <= '1';
       wait for 99 ns;
       clk <= '0';
       wait for 1 ns;
    end process;
    -- tests
    process 
    begin
        address <= "0000000";
        wait for 100 ns;
        address <= "0000001";
        wait for 100 ns;
        address <= "0000010";
        wait for 100 ns;
        address <= "0000011";
        wait for 100 ns;
        address <= "0000100";
        wait for 100 ns;
        address <= "0000101";
        wait for 100 ns;
        address <= "0000110";
        wait for 100 ns;
        address <= "0000111";
        wait for 100 ns;
        address <= "0001000";
        wait for 100 ns;
        address <= "0001001";
        wait for 100 ns;
        address <= "0001010";
        wait for 100 ns;
        wait;
    end process;
end architecture;
