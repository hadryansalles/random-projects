----------------------------------------------------------------
-- file: reg16bits_tb.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 08/17/2020
-- brief: reg16bits testbench
----------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity reg16bits_tb is
end entity;

architecture a_reg16bits_tb of reg16bits_tb is
    component reg16bits
        port (
            clk : in std_logic;
            rst : in std_logic;
            wr_en : in std_logic;
            data_in : in unsigned(15 downto 0);
            data_out : out unsigned(15 downto 0)
        );
    end component;
    signal clk, rst, wr_en : std_logic := '0';
    signal data_in, data_out : unsigned(15 downto 0) := "0000000000000000";
begin
    uut : reg16bits port map (
        clk => clk,
        rst => rst,
        wr_en => wr_en,
        data_in => data_in,
        data_out => data_out
    );
    process
    begin
        clk <= '0';
        wait for 50 ns;
        clk <= '1';
        wait for 50 ns;
    end process;
    process
    begin
        rst <= '1';
        wait for 100 ns;
        rst <= '0';
        wait;
    end process;

    process
    begin
        wait for 100 ns;
        wr_en <= '1';
        data_in <= "1111111111111111";
        wait for 100 ns;
        wr_en <= '0';
        data_in <= "1000110100000000";
        wait for 100 ns;
        wr_en <= '1';
        data_in <= "1000110100000000";
        wait for 100 ns;
        wait;
    end process;
end architecture;
