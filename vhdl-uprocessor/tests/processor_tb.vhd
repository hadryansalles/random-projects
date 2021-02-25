---------------------------------------------------------------
-- file: processor_tb.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 09/14/2020
-- brief: processor tests 
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity processor_tb is
end entity;

architecture a_processor_tb of processor_tb is
    component processor
        port (
            clk, rst : in std_logic;
            p_state : out unsigned(1 downto 0); 
            p_pc : out unsigned(11 downto 0);
            p_instr : out unsigned(15 downto 0);
            p_reg0_out : out unsigned(15 downto 0);
            p_reg1_out : out unsigned(15 downto 0);
            p_alu_out : out unsigned(15 downto 0)
        );
    end component;
    signal clk, rst : std_logic := '0';
    signal state : unsigned(1 downto 0) := "00";
    signal pc : unsigned(11 downto 0) := "000000000000";
    signal instr, reg0_out, reg1_out, alu_out : unsigned(15 downto 0) :="0000000000000000";
begin
    -- connects processor
    uut : processor port map ( 
        clk, rst, 
        state,  pc, instr, 
        reg0_out, reg1_out, alu_out 
    );
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

