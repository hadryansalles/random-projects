---------------------------------------------------------------
-- file: ram.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 10/04/2020
-- brief: ram top level 
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ram is
    port (
        clk      : in std_logic;
        address  : in unsigned(6 downto 0);
        wr_en    : in std_logic;
        data_in  : in unsigned(15 downto 0);
        data_out : out unsigned(15 downto 0)
    );
end entity;

architecture a_ram of ram is
    type mem is array (0 to 127) of unsigned(15 downto 0);
    signal ram_content : mem;
begin
    -- write to ram synchronous  
    process(clk, wr_en)
    begin
        if rising_edge(clk) then
            if wr_en='1' then
                ram_content(to_integer(address)) <= data_in;
            end if;
        end if;
    end process;
    -- read from ram asynchronous
    data_out <= ram_content(to_integer(address));
end architecture;