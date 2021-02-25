---------------------------------------------------------------
-- file: ram_tb.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 10/04/2020
-- brief: ram tests
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ram_tb is
end entity;

architecture a_ram_tb of ram_tb is
    -- ram interface
    component ram 
        port (
            clk      : in std_logic;
            address  : in unsigned(6 downto 0);
            wr_en    : in std_logic;
            data_in  : in unsigned(15 downto 0);
            data_out : out unsigned(15 downto 0)
        );
    end component;
    -- signals
    signal clk, wr_en : std_logic := '0';
    signal address    : unsigned(6 downto 0) := "0000000";
    signal data_in, data_out : unsigned(15 downto 0) := "0000000000000000";
begin
    -- connect to ram
    uut : ram port map(clk, address, wr_en, data_in, data_out);
    -- clock pulse
    process
    begin
       clk <= '1';
       wait for 50 ns;
       clk <= '0';
       wait for 50 ns;
    end process;
    -- tests
    process
    begin
        wr_en <= '1';
        -- writes
        address <= "0000000";
        data_in <= "0000000000000001";
        wait for 100 ns;
        address <= "0000001";
        data_in <= "0000000000000100";
        wait for 100 ns;
        address <= "0000010";
        data_in <= "0000000000010000";
        wait for 100 ns;
        address <= "0000100";
        data_in <= "0000000001000000";
        wait for 100 ns;
        address <= "0001000";
        data_in <= "0000000100000000";
        wait for 100 ns;
        address <= "0010000";
        data_in <= "0000010000000000";
        wait for 100 ns;
        address <= "0100000";
        data_in <= "0001000000000000";
        wait for 100 ns;
        address <= "1000000";
        data_in <= "0100000000000000";
        wait for 100 ns;
        wr_en <= '0';
        data_in <= "0000000000000000";
        -- reads
        address <= "0000000"; 
        wait for 100 ns;
        address <= "0000001"; 
        wait for 100 ns;
        address <= "0000010"; 
        wait for 100 ns;
        address <= "0000100"; 
        wait for 100 ns;
        address <= "0001000"; 
        wait for 100 ns;
        address <= "0010000"; 
        wait for 100 ns;
        address <= "0100000"; 
        wait for 100 ns;
        address <= "1000000"; 
        wait for 100 ns;
        wait;
    end process;
end architecture;