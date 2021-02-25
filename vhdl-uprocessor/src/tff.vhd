---------------------------------------------------------------
-- file: tff.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 09/02/2020
-- brief: T flip-flop
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tff is 
    -- T flip-flop interface
    port (
        clk, rst : in std_logic;
        data_out : out std_logic
    );
end entity;

architecture a_tff of tff is
    signal state : std_logic := '0';
begin
    process(clk)
    begin
        -- when the clock ticks reset the state or flip it
        if rising_edge(clk) then
            if rst = '1' then
                state <= '0';
            else
                state <= not state;
            end if;
        end if;
    end process;
    data_out <= state;
end architecture;
