----------------------------------------------------------------
-- file: reg16bits.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 08/17/2020
-- brief: single register of 16 bits
----------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity reg16bits is
    -- reg16bits interface
    port (
        clk, rst, wr_en : in std_logic;
        data_in  : in unsigned(15 downto 0);
        data_out : out unsigned(15 downto 0)
    );
end entity;

architecture a_reg16bits of reg16bits is
    -- signals
    signal reg_data : unsigned(15 downto 0) := "0000000000000000";
begin
    process (clk, rst, wr_en)
    begin
        if rst = '1' then
            reg_data <= "0000000000000000";
        elsif wr_en = '1' then
            if rising_edge(clk) then
                reg_data <= data_in;
            end if;
        end if;
    end process;
    data_out <= reg_data;
end architecture;
