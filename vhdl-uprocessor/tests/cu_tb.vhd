---------------------------------------------------------------
-- file: cu_tb.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 09/02/2020
-- brief: CU tests
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity cu_tb is
end entity;

architecture a_cu_tb of cu_tb is
    component cu
        port (
            state      :  in unsigned(1 downto 0) := "00";
            instr      :  in unsigned(15 downto 0) := "0000000000000000";
            nop_op     : out std_logic := '0';
            jr_op    : out std_logic := '0';
            jmp_op    : out std_logic := '0';
            add_op     : out std_logic := '0';
            sub_op     : out std_logic := '0';
            mul_op     : out std_logic := '0';
            cmp_op     : out std_logic := '0';
            be_op      : out std_logic := '0';
            blt_op     : out std_logic := '0';
            mov_op     : out std_logic := '0';
            const_load : out std_logic := '0';
            pc_wr_en   : out std_logic := '0';
            excpt      : out std_logic := '0';
            registers_wr_en : out std_logic := '0';
            flags_wr_en     : out std_logic := '0'
        );
    end component;
    signal state      : unsigned( 1 downto 0) := "00";
    signal instr      : unsigned(15 downto 0) := "0000000000000000";
    signal opcode     : unsigned( 3 downto 0) := "0000";
    signal nop_op     : std_logic := '0';
    signal jr_op    : std_logic := '0';
    signal jmp_op    : std_logic := '0';
    signal pc_wr_en   : std_logic := '0';
    signal excpt      : std_logic := '0';
begin
    -- connects pc
    uut : cu port map (
        state => state,
        instr => instr,
        nop_op => nop_op,
        jr_op  => jr_op,
        jmp_op => jmp_op,
        add_op => open,
        sub_op => open,
        mul_op => open,
        cmp_op => open,
        be_op  => open,
        blt_op => open,
        mov_op => open,
        const_load => open,
        pc_wr_en => pc_wr_en,
        excpt => excpt,
        registers_wr_en => open,
        flags_wr_en => open
    );
    
    opcode <= instr(15 downto 12);
    process
    begin
        state <= "00";
        instr <= B"1111_0000_0010_1000";
        wait for 50 ns;
        state <= "01";
        instr <= B"1111_0000_0010_1000";
        wait for 50 ns;
        state <= "00";
        instr <= B"0000_0000_0010_1000";
        wait for 50 ns;
        state <= "01";
        instr <= B"0000_0000_0010_1000";
        wait for 50 ns;
        state <= "00";
        instr <= B"0010_0000_0010_1000";
        wait for 50 ns;
        state <= "01";
        instr <= B"0010_0000_0010_1000";
        wait for 50 ns;
        wait;
    end process;
end architecture;

