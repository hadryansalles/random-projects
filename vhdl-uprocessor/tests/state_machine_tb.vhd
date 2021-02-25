---------------------------------------------------------------
-- file: state_machine_tb.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 09/15/2020
-- brief: state machine tests
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity state_machine_tb is
end entity;

architecture a_state_machine_tb of state_machine_tb is
    component state_machine is
        port (
            clk   :  in std_logic;
            rst   :  in std_logic;
            state : out unsigned(1 downto 0)
        );
    end component;
    signal clk, rst : std_logic;
    signal state : unsigned(1 downto 0);
begin
    uut: state_machine port map(clk, rst, state);

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
