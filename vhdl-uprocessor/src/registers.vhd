----------------------------------------------------------------
-- file: registers.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 08/19/2020
-- brief: registers bank
----------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- registers interface
entity registers is
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
end entity;

architecture a_registers of registers is
    -- reg16bits interface 
    component reg16bits
        port (
            clk : in std_logic;
            rst : in std_logic;
            wr_en : in std_logic;
            data_in : in unsigned(15 downto 0);
            data_out : out unsigned(15 downto 0)
        );
    end component;
    -- mux8x16 interface
    component mux8x16
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
    end component;
    -- demux8x1 interface
    component demux8x1
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
    end component;
    -- signals
    signal wr_enablers : unsigned(7 downto 0) := "00000000";
    signal reg_data0 : unsigned(15 downto 0) := "0000000000000000";
    signal reg_data1 : unsigned(15 downto 0) := "0000000000000000";
    signal reg_data2 : unsigned(15 downto 0) := "0000000000000000";
    signal reg_data3 : unsigned(15 downto 0) := "0000000000000000";
    signal reg_data4 : unsigned(15 downto 0) := "0000000000000000";
    signal reg_data5 : unsigned(15 downto 0) := "0000000000000000";
    signal reg_data6 : unsigned(15 downto 0) := "0000000000000000";
    signal reg_data7 : unsigned(15 downto 0) := "0000000000000000";
begin
    -- connects write enablers
    write_enablers : demux8x1 port map (
        sel => write_reg,
        data_in => wr_en,
        out0 => open, -- don't connects $0
        out1 => wr_enablers(1),
        out2 => wr_enablers(2),
        out3 => wr_enablers(3),
        out4 => wr_enablers(4),
        out5 => wr_enablers(5),
        out6 => wr_enablers(6),
        out7 => wr_enablers(7)
    );
    -- connects registers 
    reg0 : reg16bits port map (
        data_in => data_in,
        wr_en => '0',
        clk => clk,
        rst => rst,
        data_out => reg_data0
    );
    reg1 : reg16bits port map (
        data_in => data_in,
        wr_en => wr_enablers(1),
        clk => clk,
        rst => rst,
        data_out => reg_data1
    );
    reg2 : reg16bits port map (
        data_in => data_in,
        wr_en => wr_enablers(2),
        clk => clk,
        rst => rst,
        data_out => reg_data2
    );
    reg3 : reg16bits port map (
        data_in => data_in,
        wr_en => wr_enablers(3),
        clk => clk,
        rst => rst,
        data_out => reg_data3
    );
    reg4 : reg16bits port map (
        data_in => data_in,
        wr_en => wr_enablers(4),
        clk => clk,
        rst => rst,
        data_out => reg_data4
    );
    reg5 : reg16bits port map (
        data_in => data_in,
        wr_en => wr_enablers(5),
        clk => clk,
        rst => rst,
        data_out => reg_data5
    );
    reg6 : reg16bits port map (
        data_in => data_in,
        wr_en => wr_enablers(6),
        clk => clk,
        rst => rst,
        data_out => reg_data6
    );
    reg7 : reg16bits port map (
        data_in => data_in,
        wr_en => wr_enablers(7),
        clk => clk,
        rst => rst,
        data_out => reg_data7
    );
    -- selects register to read data 0
    read0 : mux8x16 port map (
        sel => read_reg0,
        in0 => reg_data0,
        in1 => reg_data1,
        in2 => reg_data2,
        in3 => reg_data3,
        in4 => reg_data4,
        in5 => reg_data5,
        in6 => reg_data6,
        in7 => reg_data7,
        data_out => read_data0
    );
    -- selects register to read data 1
    read1 : mux8x16 port map (
        sel => read_reg1,
        in0 => reg_data0,
        in1 => reg_data1,
        in2 => reg_data2,
        in3 => reg_data3,
        in4 => reg_data4,
        in5 => reg_data5,
        in6 => reg_data6,
        in7 => reg_data7,
        data_out => read_data1
    );
end architecture;
