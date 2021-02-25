---------------------------------------------------------------
-- file: cu_top_level_tb.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 09/15/2020
-- brief: CU top level tests
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity cu_top_level_tb is
end entity;

architecture a_cu_top_level_tb of cu_top_level_tb is
    component cu_top_level
        port (clk, rst : in std_logic);
    end component;
    signal clk, rst : std_logic;
begin
    -- connects pc
    uut : cu_top_level port map( clk, rst );
    process
    begin
        clk <= '1';
        wait for 50 ns;
        clk <= '0';
        wait for 50 ns;
    end process;

    process
    begin
        rst <= '1';
        wait for 100 ns;
        rst <= '0';
        wait;
    end process;
end architecture;

