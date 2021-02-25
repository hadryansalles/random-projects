---------------------------------------------------------------
-- file: cu_top_level.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 09/15/2020
-- brief: Control Unit inter connections (CU)
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity cu_top_level is
    port (clk, rst : in std_logic);
end entity;

architecture a_cu_top_level of cu_top_level is
    -- pc interface
    component pc
        port (
            clk, rst, wr_en : in std_logic;
            data_in  :  in unsigned(11 downto 0);
            data_out : out unsigned(11 downto 0)
        );
    end component;
    -- ROM interface
    component rom
        port (
            clk : in std_logic;
            address  : in unsigned(6 downto 0);
            data_out : out unsigned(15 downto 0)
        ); 
    end component;
    -- T flip-flop interface
    component tff
        port (
            clk, rst : in std_logic;
            data_out : out std_logic
        );
    end component;

    component cu
        port (
            state      :  in unsigned(1 downto 0) := "00";
            instr      :  in unsigned(15 downto 0) := "0000000000000000";
            nop_op     : out std_logic := '0';
            add_op     : out std_logic := '0';
            sub_op     : out std_logic := '0';
            mul_op     : out std_logic := '0';
            cmp_op     : out std_logic := '0';
            be_op      : out std_logic := '0';
            blt_op     : out std_logic := '0';
            mov_op     : out std_logic := '0';
            jr_op      : out std_logic := '0';
            jmp_op     : out std_logic := '0';
            ld_op      : out std_logic := '0';
            st_op      : out std_logic := '0';
            const_load : out std_logic := '0';
            pc_wr_en   : out std_logic := '0';
            excpt      : out std_logic := '0';
            registers_wr_en : out std_logic := '0';
            flags_wr_en     : out std_logic := '0'
        );
    end component;
    -- signals
    signal state  : unsigned( 1 downto 0) := "00";
    signal opcode : unsigned( 3 downto 0) := "0000";
    signal instr  : unsigned(15 downto 0) := "0000000000000000"; 
    signal pc_in  : unsigned(11 downto 0) := "000000000000";
    signal pc_out : unsigned(11 downto 0) := "000000000000";
    
    signal nop_op_s     : std_logic := '0';
    signal jmp_op_s    : std_logic := '0';
    signal pc_wr_en_s   : std_logic := '0';
    signal excpt_s      : std_logic := '0';
begin
    -- connects pc
    pc_c: pc port map (
       clk => clk,
       rst => rst,
       wr_en => pc_wr_en_s,
       data_in => pc_in,
       data_out => pc_out
   );
    -- connects ROM
    rom_c : rom port map(
                        clk => clk,
                        address => pc_out(6 downto 0),
                        data_out => instr);
    -- connects T flip-flop
    tff_c : tff port map(clk, rst, state(0));

    cu_c : cu port map ( 
        state => state,
        instr => instr,
        nop_op => nop_op_s,
        pc_wr_en => pc_wr_en_s,
        excpt => excpt_s,
        add_op => open,
        jr_op  => open, 
        jmp_op => jmp_op_s, 
        sub_op => open,
        mul_op => open,
        cmp_op => open,
        be_op  => open,
        blt_op => open,
        mov_op => open,
        ld_op => open,
        st_op => open,
        const_load => open,
        registers_wr_en => open,
        flags_wr_en => open
    );
        
    pc_in  <= instr(11 downto 0) when jmp_op_s = '1' else 
              pc_out + 1;
end architecture;
