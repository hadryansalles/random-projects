----------------------------------------------------------------
-- file: registers.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 08/19/2020
-- brief: registers back testbench
----------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity registers_tb is
end entity;

architecture a_registers_tb of registers_tb is
    -- registers interface
    component registers is
        port (
            -- control
            clk : in std_logic;
            rst : in std_logic;
            wr_en : in std_logic;
            -- register selectors
            read_reg0 : in unsigned(2 downto 0);
            read_reg1 : in unsigned(2 downto 0);
            write_reg : in unsigned(2 downto 0);
            -- data input
            data_in : in unsigned(15 downto 0);
            -- data output
            read_data0 : out unsigned(15 downto 0);
            read_data1 : out unsigned(15 downto 0)
        );
    end component;
    -- signals
    signal clk, rst, wr_en : std_logic := '0';
    signal read_reg0, read_reg1, write_reg:
        unsigned(2 downto 0) := "000";
    signal data_in, read_data0, read_data1:
        unsigned(15 downto 0) := "0000000000000000";
begin
    uut : registers port map (
        clk, rst, wr_en, 
        read_reg0, read_reg1, write_reg, 
        data_in, read_data0, read_data1
    );
    -- clock pulse
    process
    begin
        clk <= '0';
        wait for 50 ns;
        clk <= '1';
        wait for 50 ns;
    end process;
    process
    -- initial reset
    begin
        rst <= '1';
        wait for 100 ns;
        rst <= '0';
        wait;
    end process;
    process
    begin
        -- wait the initial reset
        wait for 100 ns;
        
        -- initial read from all registers should be "000000000000000" 
        wr_en <= '0';
        -- read $1 
        read_reg0 <= "001";
        read_reg1 <= "001";
        wait for 100 ns; 
        -- read $2 
        read_reg0 <= "010";
        read_reg1 <= "010";
        wait for 100 ns; 
        -- read $3 
        read_reg0 <= "011";
        read_reg1 <= "011";
        wait for 100 ns; 
        -- read $4 
        read_reg0 <= "100";
        read_reg1 <= "100";
        wait for 100 ns; 
        -- read $5 
        read_reg0 <= "101";
        read_reg1 <= "101";
        wait for 100 ns; 
        -- read $6 
        read_reg0 <= "110";
        read_reg1 <= "110";
        wait for 100 ns; 
        -- read $7 
        read_reg0 <= "111";
        read_reg1 <= "111";
        wait for 100 ns;

        -- reset read registers and write to all registers
        wr_en <= '1';
        read_reg0 <= "000";
        read_reg1 <= "000";
        -- try to write $0 = 1 (writing to $0 is not allowed) 
        write_reg <= "000";
        data_in <= "0000000000000001";
        wait for 100 ns;
        -- $1 = 2 
        write_reg <= "001";
        data_in <= "0000000000000010";
        wait for 100 ns;
        -- $2 = 3 
        write_reg <= "010";
        data_in <= "0000000000000011";
        wait for 100 ns;
        -- $3 = 4 
        write_reg <= "011";
        data_in <= "0000000000000100";
        wait for 100 ns;
        -- $4 = 5 
        write_reg <= "100";
        data_in <= "0000000000000101";
        wait for 100 ns;
        -- $5 = 6 
        write_reg <= "101";
        data_in <= "0000000000000110";
        wait for 100 ns;
        -- $6 = 7 
        write_reg <= "110";
        data_in <= "0000000000000111";
        wait for 100 ns;
        -- $7 = 8 
        write_reg <= "111";
        data_in <= "0000000000001000";
        wait for 100 ns;

        -- reset writing data and read from all registers
        write_reg <= "000";
        data_in <= "0000000000000000";
        wr_en <= '0';
        -- read $0 
        read_reg0 <= "000";
        read_reg1 <= "000";
        wait for 100 ns; 
        -- read $1 
        read_reg0 <= "001";
        read_reg1 <= "001";
        wait for 100 ns; 
        -- read $2 
        read_reg0 <= "010";
        read_reg1 <= "010";
        wait for 100 ns; 
        -- read $3 
        read_reg0 <= "011";
        read_reg1 <= "011";
        wait for 100 ns; 
        -- read $4 
        read_reg0 <= "100";
        read_reg1 <= "100";
        wait for 100 ns; 
        -- read $5 
        read_reg0 <= "101";
        read_reg1 <= "101";
        wait for 100 ns; 
        -- read $6 
        read_reg0 <= "110";
        read_reg1 <= "110";
        wait for 100 ns; 
        -- read $7 
        read_reg0 <= "111";
        read_reg1 <= "111";
        wait for 100 ns; 
        wait;
    end process;
end architecture;
