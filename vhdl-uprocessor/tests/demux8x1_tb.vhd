----------------------------------------------------------------
-- file: demux8x1_tb.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 08/19/2020
-- brief: demux8x1 testbench
----------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity demux8x1_tb is
end entity;

architecture a_demux8x1_tb of demux8x1_tb is
    -- demux8x1 interface
    component demux8x1
        port (
            sel : in unsigned(2 downto 0);
            data_in : in std_logic;
            out0 : out std_logic;
            out1 : out std_logic;
            out2 : out std_logic;
            out3 : out std_logic;
            out4 : out std_logic;
            out5 : out std_logic;
            out6 : out std_logic;
            out7 : out std_logic
        );
    end component;
    signal sel : unsigned(2 downto 0) := "000";
    signal data_in : std_logic := '0';
    signal out0, out1, out2, out3 : std_logic := '0';
    signal out4, out5, out6, out7 : std_logic := '0';
begin
    uut : demux8x1 port map (
        sel => sel,
        data_in => data_in,
        out0 => out0,
        out1 => out1,
        out2 => out2,
        out3 => out3,
        out4 => out4,
        out5 => out5,
        out6 => out6,
        out7 => out7
    );
    process
    begin
        data_in <= '1';
        sel <= "000";
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
