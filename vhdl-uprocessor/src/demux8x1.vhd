----------------------------------------------------------------
-- file: demux8x1.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 08/19/2020
-- brief: demux with 8 outputs of 1 bit each
----------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- demux8x1 interface
entity demux8x1 is
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
end entity;

architecture a_demux8x1 of demux8x1 is
begin
    out0 <= data_in when sel = "000" else '0';
    out1 <= data_in when sel = "001" else '0';
    out2 <= data_in when sel = "010" else '0';
    out3 <= data_in when sel = "011" else '0';
    out4 <= data_in when sel = "100" else '0';
    out5 <= data_in when sel = "101" else '0';
    out6 <= data_in when sel = "110" else '0';
    out7 <= data_in when sel = "111" else '0';
end architecture;
