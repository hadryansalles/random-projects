---------------------------------------------------------------
-- file: tff_tb.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 09/02/2020
-- brief: T flip-flop tests
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tff_tb is
end entity;

architecture a_tff_tb of tff_tb is
    component tff is
        -- T flip-flop interface
        port (
            clk : in std_logic;
            rst : in std_logic;
            data_out : out std_logic
        );
    end component;
    -- signals
    signal clk, rst, data_out : std_logic := '0';
begin
    -- connect tff interface
    uut : tff port map(clk, rst, data_out);
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
        wait for 100 ns;
        wait for 100 ns;
        wait for 100 ns;
        rst <= '1';
        wait for 200 ns;
        rst <= '0';
        wait for 200 ns;
        wait;
    end process;
end architecture;
