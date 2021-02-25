----------------------------------------------------------------
-- file: mux8x16.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 08/19/2020
-- brief: mux with 8 outputs of 16 bits each
----------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- mux8x16 interface
entity mux8x16 is
    port (
        sel : in unsigned(2 downto 0);
        in0 : in unsigned(15 downto 0);
        in1 : in unsigned(15 downto 0);
        in2 : in unsigned(15 downto 0);
        in3 : in unsigned(15 downto 0);
        in4 : in unsigned(15 downto 0);
        in5 : in unsigned(15 downto 0);
        in6 : in unsigned(15 downto 0);
        in7 : in unsigned(15 downto 0);
        data_out : out unsigned(15 downto 0)
    );
end entity;

architecture a_mux8x16 of mux8x16 is
begin
    data_out <= in0 when sel="000" else
        in1 when sel="001" else
        in2 when sel="010" else
        in3 when sel="011" else
        in4 when sel="100" else
        in5 when sel="101" else
        in6 when sel="110" else
        in7 when sel="111" else
        "0000000000000000";
end architecture;
