---------------------------------------------------------------
-- file: mux2x16.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 08/13/2020
-- brief: mux2x16 testbench
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity mux2x16_tb is
end entity;

architecture a_mux2x16_tb of mux2x16_tb is
    component mux2x16
        port (
            sel : std_logic;
            in0 : in unsigned(15 downto 0);
            in1 : in unsigned(15 downto 0);
            data_out : out unsigned(15 downto 0)
        );
    end component;
    signal sel : std_logic := '0';
    signal in0, in1, data_out : unsigned(15 downto 0) := "0000000000000000";
begin
    uut : mux2x16 port map (
        sel => sel,
        in0 => in0,
        in1 => in1,
        data_out => data_out
    );
    process
    begin
        in0 <= "0000000000000001";
        in1 <= "0000000000000010";
        sel <= '0';
        wait for 100 ns;
        sel <= '1';
        wait for 100 ns;
        wait;
    end process;
end architecture;
