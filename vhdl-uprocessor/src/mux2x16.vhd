---------------------------------------------------------------
-- file: mux2x16.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 08/13/2020
-- brief: mux 2 inputs of 16 bits each
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity mux2x16 is
    port (
        sel : std_logic;
        in0 : in unsigned(15 downto 0);
        in1 : in unsigned(15 downto 0);
        data_out : out unsigned(15 downto 0)
    );
end entity;

architecture a_mux2x16 of mux2x16 is
begin
    data_out <= in0 when sel='0' else
                in1 when sel='1' else
                "0000000000000000";
end architecture;
