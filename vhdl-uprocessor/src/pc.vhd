----------------------------------------------------------------
-- file: pc.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 08/17/2020
-- brief: single register of 16 bits
----------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity pc is
    port (
        clk, rst, wr_en : in std_logic;
        data_in  : in unsigned(11 downto 0);
        data_out : out unsigned(11 downto 0)
    );
end entity;

architecture a_pc of pc is
    -- signals
    signal address : unsigned(11 downto 0) := "000000000000";
begin
    process (clk, rst, wr_en)
    begin
        if rst = '1' then
            address <= "000000000000";
        elsif wr_en = '1' then
            if rising_edge(clk) then
                address <= data_in;
            end if;
        end if;
    end process;
    data_out <= address;
end architecture;
